@Echo off
Echo %1 �������Ŀ¼
Echo %2 �ǹ�������

set myDir=%1
set myDir=%myDir:"=%

set mypj=%2
set mypj=%mypj:"=%

set CC65_HOME=%myDir%_APP\_cc65
set Path=%myDir%_APP\_cc65\bin

Echo CC65_HOME=%CC65_HOME%
Echo Path=%Path%

Echo .
Echo ��һ����ɾ��ģ����NES
del *.o
del *.nes
ar65 d nes.lib crt0.o

Echo .
Echo �ڶ���������Ӳ��ģ�飬����ӵ���
ca65 -t nes crt0.s
ar65 a nes.lib crt0.o

Echo .
Echo ������������C�������ӿ⣬����nes
echo on
cl65 -C nes.cfg -t nes -o "%mypj%.nes" -I "%CC65_HOME%\include" tetris.c

@Echo off
Echo .
pause