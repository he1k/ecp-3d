clear all;
close all;
clc;
port = 'COM15';
path = "C:\Users\farfar\Documents\Git\ecp-3d\software\teensy\teensy-ecp\src\serial_commands.h";
baudrate = 115200;
%% Open serialport
s = serialport(port,baudrate)
%% Read file with commands
fid = fopen(path);
tline = fgetl(fid);
while ischar(tline)
    sp = split(tline);
    if(length(sp) == 3)
        name = sp{2};
        val = str2double(sp{3});
        assignin('base', name,val);
    end
    tline = fgetl(fid);
end
%% Test
write(s,[CMD_START, LINE_FEED],'uint8')
pause(1);
read(s,2,'uint8')

