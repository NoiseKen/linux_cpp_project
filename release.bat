set API=linux_work
set RAR=C:\Program Files\WinRAR

mkdir %API%
mkdir %API%\cppProj
mkdir %API%\kw_debug

REM xcopy /d %API%\*.bat %API%\.
xcopy * .\%API%\.
xcopy /e .\cppProj %API%\cppProj\.
xcopy /e .\kw_debug %API%\kw_debug\.

"%RAR%\winrar.exe" m -afzip %API%.zip .\%API%

