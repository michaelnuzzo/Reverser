% Custom testbench tool for verification.

%%
clc;clearvars;close all;

%% plugin
% Set up the plugin parameters

bufsize = 2048;
fs = 44100;

plugin = loadAudioPlugin('/Users/michaelnuzzo/Library/Audio/Plug-Ins/VST3/Reverser19.vst3');
plugin.Time = 1000;
plugin.DryWet = 0;

%% data
% Set up the environment
time = 0:1/fs:10;

% audio = [sin(time*2*pi*440)' sin(time*2*pi*440)']; % sin


% audio = 2*(rand(length(time),2)-0.5); % noise

audio = ones(length(time),2);
audio(:,1) = -1;

% [audio,fs] = audioread('/Users/michaelnuzzo/Music/iTunes/iTunes Media/Music/Kacey Musgraves/Golden Hour/06 Love Is A Wild Thing.m4a');
% audio = audio(1:fs*10,:);

%% process

dry = [0,0];
processed = [0,0];

% h = animatedline;
% profile on

% Start the DSP loop
for k = 1:(length(audio)/bufsize)-1

    % Stores each buffer input
    in = audio((k*bufsize)+(1:bufsize),:);
    
    % Saves the input via buffers
    dry = [dry;in];
    
    % Processing
    out = process(plugin, in);
    
    % Saves the output via buffers
    processed = [processed;out];
%     addpoints(h,1:length(processed),processed(:,1));
%     drawnow
end