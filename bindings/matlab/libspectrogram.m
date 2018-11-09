function result = libspectrogram(signal,varargin)
% Interface for computing spectrograms using libspectrogram
%
% Usage: result = libspectrogram( signal, [key-value parameters])
%
% Input: 
%   signal - A 1-dimensional array of single of double type
%
% Parameters: 
%   SampleRate    - The sampling rate of the input signal (Hz)
%   PaddingMode   - Whether to zero-pad the input signal to nearest window ('TRUNCATE' or 'PAD')
%   WindowLength  - Length of each window (samples)
%   WindowOverlap - Length of overlap between consecutive windows (samples)
%   WindowType    - Windowing function to use. Valid choices: 'RECTANGULAR', 
%                   'TRIANGULAR', 'BARTLETT', 'HANN', 'WELCH', 'HAMMING', 
%                   'BLACKMAN', 'NUTTALL', 'BLACKMAN_NUTTALL', 'BLACKMAN_HARRIS
%
% Output:
%   Result - A struct containing the following fields:
%       time  - [1 x T] vector of times (in seconds)
%       freq  - [1 x F] vector of frequencies (in Hz)
%       power - [F x T] matrix of power at each time and frequency
%       phase - [F x T] matrix of phase at each time and frequency
%
% Example:
%   % Compute spectrogram
%   result = libspectrogram(rand(10000,1), 'SampleRate',10, 'PaddingMode','PAD', ...
%                           'WindowLength',100, 'WindowOverlap',50, 'WindowType','HAMMING');
%   % Draw power
%   figure; imagesc(result.time, result.freq, result.power);

%% Parse inputs
valid_windows = {'RECTANGULAR'
                 'TRIANGULAR'
                 'BARTLETT'
                 'HANN'
                 'WELCH'
                 'HAMMING'
                 'BLACKMAN'
                 'NUTTALL'
                 'BLACKMAN_NUTTALL'
                 'BLACKMAN_HARRIS'};

valid_paddings = {'TRUNCATE','PAD'};
             
iswindow = @(x) any(strcmpi(valid_windows,x));
ispadding = @(x) any(strcmpi(valid_paddings,x));
             
p = inputParser;

% Properties of the data
addParameter(p,'SampleRate',[],@isnumeric);

% Properties of the transform
addParameter(p,'WindowType',[],iswindow);
addParameter(p,'PaddingMode',[],ispadding);
addParameter(p,'WindowLength',[],@isnumeric);
addParameter(p,'WindowOverlap',[],@isnumeric);
addParameter(p,'TransformLength',[],@isnumeric);

parse(p,varargin{:});

%% Do some error checks
loc = which('libspectrogram_mex');
if isempty(loc)
    error(['Could not find libspectrogram_mex.' mexext '. Please build it and try again.']);
end
if isempty(p.Results.SampleRate)
    error('Please specify ''SampleRate''');
end
if isempty(p.Results.WindowType)
    error('Please specify ''WindowType''');
end
if isempty(p.Results.PaddingMode)
    error('Please specify ''PaddingMode''');
end
if isempty(p.Results.WindowLength)
    error('Please specify ''WindowLength''');
end
if isempty(p.Results.WindowOverlap)
    error('Please specify ''WindowOverlap''');
end
if isempty(p.Results.TransformLength)
    error('Please specify ''TransformLength''');
end

%% Setup and execute
config = struct;
config.sample_rate = p.Results.SampleRate;
config.window_type = upper(p.Results.WindowType);
config.padding_mode = upper(p.Results.PaddingMode);
config.window_length = p.Results.WindowLength;
config.window_overlap = p.Results.WindowOverlap;
config.transform_length = p.Results.TransformLength;

result = libspectrogram_mex(signal,config);

end