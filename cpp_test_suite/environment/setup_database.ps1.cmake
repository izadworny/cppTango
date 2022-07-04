if ((Get-Command "docker.exe" -ErrorAction SilentlyContinue) -eq $null)
{
  Write-Host "Can not run the tests as docker is missing"
  exit
}

if($args[0] -ne $null)
{
	$mysql_container=$args[0]
}
else
{
	$mysql_container="mysql_db"
}
if($args[1] -ne $null)
{
	$tango_container=$args[1]
}
else
{
	$tango_container="tango_cs"
}
&{
	docker.exe network create tango-net
} | Out-Null

&{docker.exe run `
    --name "$mysql_container" `
    --rm `
    --network tango-net `
    --network-alias mysql_db `
    -e MYSQL_ROOT_PASSWORD=root `
    -e MYSQL_INITDB_SKIP_TZINFO=1 `
    -d `
    registry.gitlab.com/tango-controls/docker/mysql:5.16-mysql-5 `
    --sql-mode="" `
    --innodb=OFF `
    --default-storage-engine=MyISAM
} | Out-Null

&{docker.exe run `
    --name "$tango_container" `
    --rm `
	-p 10000:10000 `
    --network tango-net `
    -e TANGO_HOST=127.0.0.1:10000 `
    -e MYSQL_HOST=mysql_db:3306 `
    -e MYSQL_USER=tango `
    -e MYSQL_PASSWORD=tango `
    -e MYSQL_DATABASE=tango `
    -d `
    registry.gitlab.com/tango-controls/docker/tango-db:5.16 
} | Out-Null

$tango_ipaddr="$(docker inspect `
    -f '{{ .NetworkSettings.IPAddress }}' `
    "$tango_container")"

Write-Host $tango_ipaddr

$env:TANGO_HOST="127.0.0.1:10000"
<#
$ping = &{docker run --rm `
    --read-only `
    --network=host `
    -e TANGO_HOST="$env:TANGO_HOST" `
    --entrypoint /tango/bin/tango_admin `
registry.gitlab.com/tango-controls/docker/tango-db:5.16 --ping-database 30}

Write-Host $ping

if (!$ping)
{
    Write-Host "Failed to ping database"
    exit
}
#>
