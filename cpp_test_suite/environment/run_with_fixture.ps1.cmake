$TANGO_TEST_CASE_SKIP_FIXTURE = $false

if ( $args.Count -lt 1 )
{
    Write-Host "usage: $0 <executable> [args...]"
    pause
}

if ((Get-Command "docker.exe" -ErrorAction SilentlyContinue) -eq $null)
{
  Write-Host "Can not run the tests as docker is missing"
  pause
}

# Construct unique name for this test execution. We are using current date with
# nanosecond precision to ensure that subsequent test runs will get a new name.
$tc_program= $args[0]
$tc_program_path = Get-ChildItem $tc_program
Write-Host $tc_program
$date = Get-Date -Format "yyyyMMdd.HHmmss.fffffff"
$tc_run_name=$tc_program_path.Basename+"_"+$date
Write-Host $tc_run_name

$null, $args = $args
Write-Host $args

if ( $args.Count -gt 0 )
{
	$arg=$args[0]
	
	if ($args.count -eq 1)
	{
		$arg = $args
	}
	
	if ($arg -eq "--skip-fixtures")
	{
		$TANGO_TEST_CASE_SKIP_FIXTURE = $true
		$null, $args = $args
	}
}
Write-Host $TANGO_TEST_CASE_SKIP_FIXTURE
Write-Host $args


$tc_mysql_container="mysql_db_"+$tc_run_name
$tc_tango_container="tango_cs_"+$tc_run_name


# This path must be exported as other scripts use it for e.g. storing logs.

$env:TANGO_TEST_CASE_DIRECTORY="@PROJECT_BINARY_DIR@/test_results/"+$tc_run_name
$TANGO_TEST_CASE_DIRECTORY=$env:TANGO_TEST_CASE_DIRECTORY
mkdir -p "$TANGO_TEST_CASE_DIRECTORY"


# Cleanup function. This is attached to the EXIT signal.
function Teardown
{
    $tc_exit_code="$?"

    # Synchronously stop all running device servers. We need their exit codes.
	&"@CMAKE_CURRENT_BINARY_DIR@/kill_server.ps1"

    # Stop docker containers from a background subshell. We do not need to wait
    # for them. Also close stdout and stderr to allow ctest finish the test early.
    &{
        (docker.exe stop "$tc_tango_container" "$tc_mysql_container" | Out-Null)
    }
	if(Test-Path -Path "$TANGO_TEST_CASE_DIRECTORY/server_pids" -PathType Leaf)
	{
		# Wait up to 5s for device servers to produce files with exit status.
		$num_servers= (Get-Content "$TANGO_TEST_CASE_DIRECTORY/server_pids" | Measure-Object -Line).Lines

	}
	else
	{
		$num_servers=0
	}
			
	for ($i = 1; $i -le 50; $i++)
	{
		$exit_code_files = Get-ChildItem -Path $TANGO_TEST_CASE_DIRECTORY -Recurse -Filter '*_*_*_exit_code.txt'
		$num_exit_code_files = ($exit_code_files | Measure-Object).Count
		if ($num_servers -eq $num_exit_code_files)
		{
			break
		}
		Start-Sleep -Seconds 0.1
	}
	
    $failure=$false

    if ( $num_servers -ne $num_exit_code_files )
	{
		Add-Content -Path "$TANGO_TEST_CASE_DIRECTORY}/testcase.log" "Timeout waiting for server exit status"
        $failure=$true
    }

    if ( $num_exit_code_files -ge 1 )
	{
		Foreach ($file in $exit_code_files)
		{
			$server_exit_code = Get-Content $file
            if ($server_exit_code -ne 0)
			{
				$bn=($file).Basename
				Add-Content -Path "$TANGO_TEST_CASE_DIRECTORY}/testcase.log" "Non-zero exit status in $bn : $server_exit_code"
                $failure=$true
			}
		}
    }

    # Overwrite script exit code only if it was successful. This is needed
    # to fail the testcase if device server did not exit cleanly. The original
    # exit code can always be recovered from the testcase_exit_code.txt file.
    if (($failure) -and ($tc_exit_code -eq 0))
	{
        exit
    }
}


if(!$TANGO_TEST_CASE_SKIP_FIXTURE)
{
	trap {"An error occured"}
    Teardown
	
	. "@CMAKE_CURRENT_BINARY_DIR@/setup_database.ps1" $tc_mysql_container $tc_tango_container
    & "@CMAKE_CURRENT_BINARY_DIR@/setup_devices.ps1"
}

&$tc_program $args 2>&1 | Add-Content -Path "${TANGO_TEST_CASE_DIRECTORY}/testcase.log"
$tc_exit_code="$?"

Add-Content -Path "${TANGO_TEST_CASE_DIRECTORY}/testcase_exit_code.txt" $tc_exit_code

