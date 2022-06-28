mkdir -p "${TANGO_TEST_CASE_DIRECTORY}" # Must be set by the calling process.

$instance=$args[0]
if($args[1] -ne $null)
{
	$server=$args[1]
}
else
{
	$server="DevTest"
}
if($args[2] -ne $null)
{
	$server_path=$args[2]
}
else
{
	$server_path="cpp_test_ds"
}

Write-Host "Starting $server/$instance"
$out_log="$TANGO_TEST_CASE_DIRECTORY/$server"+"_"+"$instance"+".log"

#Write-Host "server path $server_path"
#New-Item -Path $out_log -ItemType File

$server_process=Start-Process @PROJECT_BINARY_DIR@/$server_path/Release/$server -ArgumentList "$instance", "-v5" -RedirectStandardOutput $out_log -PassThru
	
$server_pid=($server_process).Id

# Store PID in a file so that the process can be shut down later.
Add-Content -Path "$TANGO_TEST_CASE_DIRECTORY/server_pids" "$server_pid"

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

$instance_lower="$server/$instance".ToLower()

&{docker run --rm `
    --read-only `
    --network=host `
    -e TANGO_HOST="$env:TANGO_HOST" `
    --entrypoint /tango/bin/tango_admin `
registry.gitlab.com/tango-controls/docker/tango-db:5.16 --ping-device "dserver/$instance_lower" 7}