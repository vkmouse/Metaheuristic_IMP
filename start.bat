set dataset_path=Dataset\\SampleDataset\\soc-Epinions1_0.txt
set iteration=100000
set MaxOV=100000
set PerOV=2000
set Numrun=2
set NumSeed=200

set neighbor=7
set maxTemperature=10
set minTemperature=0.001

REM main.exe hc %dataset_path% %iteration% %MaxOV% %PerOV% %Numrun% %NumSeed%
main.exe sa %dataset_path% %iteration% %MaxOV% %PerOV% %Numrun% %NumSeed% %neighbor% %maxTemperature% %minTemperature%