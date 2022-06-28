
if(Test-Path -Path "$TANGO_TEST_CASE_DIRECTORY/server_pids" -PathType Leaf)
{
	$pids=[string]::Join(",", (Get-Content "$TANGO_TEST_CASE_DIRECTORY/server_pids"))

	Write-Host "Killing PIDs: $pids"

	Stop-Process -Id $pids -Force
}
