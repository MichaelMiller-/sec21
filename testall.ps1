[CmdletBinding()]
Param(
    ## relativ path to testcase directory
    [string]$path = "build\tests\Release" 
)

Write-Output "run all tests in: $path"

Get-ChildItem $path -Filter test*.exe | Foreach-Object { 
	$_.FullName
	Start-Process $_.FullName -wait -NoNewWindow 
}