set dataset_path=Dataset\\SampleDataset\\soc-Epinions1_0.txt
set iteration=600000
set MaxOV=600000
set PerOV=12000
set Numrun=2
set NumSeed=200

set neighbor=7
set maxTemperature=10
set minTemperature=0.001

set NumCms=8
set Pc=0.1
set Pm=1.0

REM main.exe hc %dataset_path% %iteration% %MaxOV% %PerOV% %Numrun% %NumSeed%
REM main.exe sa %dataset_path% %iteration% %MaxOV% %PerOV% %Numrun% %NumSeed% %neighbor% %maxTemperature% %minTemperature%
main.exe ga %dataset_path% %iteration% %MaxOV% %PerOV% %Numrun% %NumSeed% %NumCms% %Pc% %Pm%