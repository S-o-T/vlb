from dataset import SequenceDataset
import urllib
import tarfile
import os


class W1BS_Dataset(SequenceDataset):

    def __init__(self,root_dir = './datasets/', download_flag = False):
        super(W1BS_Dataset,self).__init__(name = 'W1BS', root_dir = root_dir, download_flag = download_flag)

    def download(self):
        try:
            os.stat(self.root_dir)
        except:
            os.mkdir(self.root_dir)

        try:
            os.stat('{}{}'.format(self.root_dir,self.name))
        except:
            os.mkdir('{}{}'.format(self.root_dir,self.name))

        download_url = "{}.tar.gz".format(self.url)
        download_filename = "{}/{}.tar.gz".format(self.root_dir, self.name)
        try:
            urllib.urlretrieve(download_url,download_filename)
            tar = tarfile.open(download_filename)
            tar.extractall('{}'.format(self.root_dir))
            tar.close()
            os.remove(download_filename)
        except:
            print('Cannot download from {}.'.format(download_url))
    
    def read_image_data(self):
        self.read_image_data_vggh()

    def read_link_data(self):
        self.read_link_data_vggh()
