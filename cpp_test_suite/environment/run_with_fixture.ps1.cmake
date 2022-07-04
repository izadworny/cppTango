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

function Kill_Devices
{
	param (
	    [System.Collections.Generic.List[System.Diagnostics.Process]]$processes
	)
	Write-Host $processes.count
	if ($processes.Count -ne 0)
	{
	    foreach ($process in $processes)
	    {
			Write-Host $process.id
			Stop-Process $process
	    }
	}
	else{
	    if(Test-Path -Path "$TANGO_TEST_CASE_DIRECTORY/server_pids" -PathType Leaf)
        {
			$pids=[string]::Join(",", (Get-Content "$TANGO_TEST_CASE_DIRECTORY/server_pids"))

			Write-Host "Killing PIDs: $pids"

			Stop-Process -Id $pids -Force
        }
	}
}

# Cleanup function. This is attached to the EXIT signal.
function Teardown
{
	param (
	    [System.Collections.Generic.List[System.Diagnostics.Process]]$processes
	)
    $tc_exit_code="$?"

    # Synchronously stop all running device servers. We need their exit codes.
	Kill_Devices $processes

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
		Add-Content -Path "$TANGO_TEST_CASE_DIRECTORY/testcase.log" "Timeout waiting for server exit status"
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
				Add-Content -Path "$TANGO_TEST_CASE_DIRECTORY/testcase.log" "Non-zero exit status in $bn : $server_exit_code"
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
function Start_Server
{
	param (
	    [Parameter(Mandatory)]
	    [string]$instance,
		[ValidateNotNullOrEmpty()]
		[string]$server = "DevTest",
		[ValidateNotNullOrEmpty()]
		[string]$server_path = "cpp_test_ds"
	)
	$null = New-Item "${TANGO_TEST_CASE_DIRECTORY}" -ItemType Directory -Force # Must be set by the calling process.
    
    Write-Host "Starting $server/$instance"
    $out_log="$TANGO_TEST_CASE_DIRECTORY/$server"+"_"+"$instance"+".log"
    
    #Write-Host "server path $server_path"
    #New-Item -Path $out_log -ItemType File
    
    $server_process=Start-Process @PROJECT_BINARY_DIR@/$server_path/Release/$server -ArgumentList "$instance", "-v5" -RedirectStandardOutput $out_log -PassThru
    	
    $server_pid=($server_process).Id
    
    # Store PID in a file so that the process can be shut down later.
    Add-Content -Path "$TANGO_TEST_CASE_DIRECTORY/server_pids" "$server_pid"
    
    $job = Start-Job -ScriptBlock {
        if($server_process -ne $null)
        {
        	if(!$server_process.HasExited)
        	{
        		$server_process.WaitForExit()
        	}
        }
        $server_exit_code=$server_process.ExitCode
        $exit_log="$TANGO_TEST_CASE_DIRECTORY/$server"+"_"+"$instance"+"_"+"$server_pid"+"_exit_code.txt"
        Add-Content -Path $exit_log "$server_exit_code"
    	exit $server_exit_code
    }
    
    $instance_lower="$server/$instance".ToLower()
    
    &{docker run --rm `
        --read-only `
        --network=host `
        -e TANGO_HOST="$env:TANGO_HOST" `
        --entrypoint /tango/bin/tango_admin `
    registry.gitlab.com/tango-controls/docker/tango-db:5.16 --ping-device "dserver/$instance_lower" 7}
	
	return $server_process
}

function Setup_Devices
{
	if($TANGO_TEST_CASE_DIRECTORY -ne $null)
    {
    	$env:TANGO_TEST_CASE_DIRECTORY=$TANGO_TEST_CASE_DIRECTORY
    }
    else
    {
    	$env:TANGO_TEST_CASE_DIRECTORY="."
    }
    
    @PROJECT_BINARY_DIR@/new_tests/Release/conf_devtest.exe @SERV_NAME@/@INST_NAME@ `
        @DEV1@ `
        @DEV2@ `
        @DEV3@ `
        @DEV1_ALIAS@ `
        @ATTR_ALIAS@ `
        @FWD_DEV@ `
        @DEV20@ 2>&1 | Add-Content -Path "$TANGO_TEST_CASE_DIRECTORY/conf_devtest.log"
       
    Start_Server "@INST_NAME@" "DevTest" "cpp_test_ds"
    Start_Server "@INST_NAME@" "FwdTest" "cpp_test_ds/fwd_ds"
}

$servers_process = [System.Collections.Generic.List[System.Diagnostics.Process]]::new()

if(!$TANGO_TEST_CASE_SKIP_FIXTURE)
{
	trap {"An error occured"}
    Teardown $servers_process
	
	. "@CMAKE_CURRENT_BINARY_DIR@/setup_database.ps1" $tc_mysql_container $tc_tango_container
    $servers_process = Setup_Devices
}

&$tc_program $args 2>&1 | Add-Content -Path "$TANGO_TEST_CASE_DIRECTORY/testcase.log"
$tc_exit_code=$LastExitCode

Add-Content -Path "$TANGO_TEST_CASE_DIRECTORY/testcase_exit_code.txt" $tc_exit_code

Teardown $servers_process

exit $tc_exit_code
