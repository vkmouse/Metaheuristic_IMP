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

set num_searcher=4
set num_region=4
set num_sample=2
set num_player=3

set n1=4
set n2=4
set Pi=0.8
set Fb=0
set K=3
set Fa=0.01
set Fd=0.1
set Pd=0.1

REM main.exe hc %dataset_path% %iteration% %MaxOV% %PerOV% %Numrun% %NumSeed%
REM main.exe sa %dataset_path% %iteration% %MaxOV% %PerOV% %Numrun% %NumSeed% %neighbor% %maxTemperature% %minTemperature%
REM main.exe ga %dataset_path% %iteration% %MaxOV% %PerOV% %Numrun% %NumSeed% %NumCms% %Pc% %Pm%
REM main.exe cro %dataset_path% %iteration% %MaxOV% %PerOV% %Numrun% %NumSeed% %n1% %n2% %Pi% %Fb% %K% %Fa% %Fd% %Pd%
REM main.exe se %dataset_path% %iteration% %MaxOV% %PerOV% %Numrun% %NumSeed% %num_searcher% %num_region% %num_sample% %num_player% 