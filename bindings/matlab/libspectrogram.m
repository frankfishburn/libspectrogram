function result = libspectrogram(data,varargin)
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

%% Setup and execute
config = struct;
config.sample_rate = p.Results.SampleRate;
config.window_type = p.Results.WindowType;
config.padding_mode = p.Results.PaddingMode;
config.window_length = p.Results.WindowLength;
config.window_overlap = p.Results.WindowOverlap;

result = libspectrogram_mex(data,config);

end