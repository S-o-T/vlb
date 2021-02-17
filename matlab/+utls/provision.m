function downloaded = provision( url, tgt_dir, varargin )
p = inputParser;
addParameter(p, 'doneName', '.download.done', @isstr);
addParameter(p, 'override', false, @islogical);
addParameter(p, 'forceExt', '', @isstr);
parse(p, varargin{:}); opts = p.Results;

if exist(url, 'file'), url = fileread(url); end;
url = strtrim(url);
downloaded = false;
done_file = fullfile(tgt_dir, opts.doneName);
if ~exist(tgt_dir, 'dir'), mkdir(tgt_dir); end
if exist(done_file, 'file') && ~opts.override, return; end;
unpack(url, tgt_dir, opts);
downloaded = true;
create_done(done_file);
end

function create_done(done_file)
f = fopen(done_file, 'w'); fclose(f);
fprintf('To reprovision, delete %s.\n', done_file);
end

function unpack(url, tgt_dir, opts)
[~,filename,ext] = fileparts(url);
if opts.forceExt, ext = opts.forceExt; end
tgt_file = fullfile(tgt_dir, [filename, ext]);
fprintf(isdeployed+1, ...
  'Downloading %s -> %s, this may take a while...\n',...
  url, tgt_file);

download(url, tgt_file);

fprintf(isdeployed+1, ...
  'Unpacking %s -> %s, this may take a while...\n',...
  tgt_file, tgt_dir);
switch ext
  case {'.tar', '.gz'}
    untar(tgt_file, tgt_dir);
  case '.zip'
    unzip(tgt_file, tgt_dir);
  otherwise
    error('Unknown archive %s', ext);
end
end


function outfile = download(url, target_file)
wgetCommand = 'wget --no-check-certificate %s -O %s'; % Command for downloading archives

[distDir, ~, ~] = fileparts(target_file);
mkdir(distDir);

% test if wget works
[status, ~] = system('wget --help');
if status ~= 0
  warning('WGET not found, using MATLAB.');
  outfile = websave(target_file, url);
  return;
end
  
wgetC = sprintf(wgetCommand, url, target_file);

[status, msg] = system(wgetC,'-echo');
if status ~= 0
  error('Error downloading: %s',msg);
end
end