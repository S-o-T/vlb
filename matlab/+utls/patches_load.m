function patches = patches_load(impath)
%patches_load Load and cut patch file

if ~exist(impath, 'file'), error('Patch file %s does not exist.', impath); end;
patches = imread(impath);
npatches = size(patches, 1) ./ size(patches, 2);
assert(mod(npatches, 1) == 0, 'Invalid patch image height.');
patches = mat2cell(patches, size(patches, 2)*ones(1, npatches), size(patches, 2));
patches = permute(patches, [2, 3, 4, 1]);
patches = cell2mat(patches);
