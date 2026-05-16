# Script PowerShell pour générer un fragment WiX v4 avec tous les fichiers d'un dossier

param(
    [string]$SourceDir = "build\out",
    [string]$ComponentGroupId = "MyFiles",
    [string]$DirectoryId = "INSTALLFOLDER"
)

$files = Get-ChildItem -Path $SourceDir -File -Recurse

$xml = @"
<?xml version="1.0" encoding="UTF-8"?>
<Wix xmlns="http://wixtoolset.org/schemas/v4/wxs">
  <Fragment>
    <ComponentGroup Id="$ComponentGroupId">
"@

foreach ($file in $files) {
    $relativePath = $file.FullName.Replace((Resolve-Path $SourceDir).Path + "\", "").Replace("\", "/")
    $fileId = "File_" + ($relativePath.Replace("/", "_").Replace(".", "_").Replace("-", "_"))
    $xml += "`n      <File Id=`"$fileId`" Source=`"`$($SourceDir)\$relativePath`" />"
}

$xml += @"

    </ComponentGroup>
  </Fragment>
</Wix>
"@

$xml | Out-File -FilePath "Fragment.wxs" -Encoding UTF8

Write-Host "Fragment WiX généré : Fragment.wxs"