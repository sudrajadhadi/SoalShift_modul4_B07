#define FUSE_USE_VERSION 28
#include <fuse.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <dirent.h>
#include <errno.h>
#include <sys/time.h>

static const char *dirpath = "/home/xhazimix/shift4";

int i,j;
char *namanya;
char caesar[94] = "qE1~ YMUR2\"`hNIdPzi%^t@(Ao:=CQ,nx4S[7mHFye#aT6+v)DfKL$r?bkOGB>}!9_wV']jcp5JZ&Xl|\\8s;g<{3.u*W-0";

void caesar_encrypt(char *data){
	if(!strcmp(data, ".")) return;
	if(!strcmp(data, "..")) return;
	for(i=0; i<strlen(data); i++){
		for(j=0; j<93; j++){
			if(data[i] == caesar[j]){
				data[i] = caesar[(j+17)%94];
				break;
			}
		}
	}
}

void caesar_decrypt(char *data){
	if(!strcmp(data, ".")) return;
	if(!strcmp(data, "..")) return;
	for(i=0; i<strlen(data); i++){
		for(j=0; j<100; j++){
			if(data[i] == caesar[j]){
				data[i] = caesar[((j-17)+94)%94];
				break;
			}
		}
	}
}

static int xmp_getattr(const char *path, struct stat *stbuf)
{
	char pathnya[100];
	sprintf(pathnya, "%s", path);
	caesar_encrypt(pathnya);

  	int res;
	char fpath[1000];
	sprintf(fpath,"%s%s",dirpath,pathnya);
	res = lstat(fpath, stbuf);

	if (res == -1)
		return -errno;

	return 0;
}

static int xmp_access(const char *path, int mask)
{
	char pathnya[100];
	sprintf(pathnya, "%s", path);
	caesar_encrypt(pathnya);

	int res;

  	char fpath[1000];
  	sprintf(fpath,"%s%s",dirpath,pathnya);
	res = access(fpath, mask);
	if (res == -1)
		return -errno;

	return 0;
}

static int xmp_readdir(const char *path, void *buf, fuse_fill_dir_t filler,
		       off_t offset, struct fuse_file_info *fi)
{
	char pathnya[100];
	sprintf(pathnya, "%s", path);
	caesar_encrypt(pathnya);

  	char fpath[1000];
	sprintf(fpath, "%s%s",dirpath,pathnya);
	int res = 0;

	DIR *dp;
	struct dirent *de;

	(void) offset;
	(void) fi;

	dp = opendir(fpath);
	if (dp == NULL)
		return -errno;

	while ((de = readdir(dp)) != NULL) {
		struct stat st;
		memset(&st, 0, sizeof(st));
		st.st_ino = de->d_ino;
		st.st_mode = de->d_type << 12;

    		namanya=de->d_name;
		caesar_decrypt(namanya);

		res = (filler(buf, namanya, &st, 0));
			if(res!=0) break;
	}

	closedir(dp);
	return 0;
}

static int xmp_read(const char *path, char *buf, size_t size, off_t offset,
		    struct fuse_file_info *fi)
{
	char pathnya[100];
	sprintf(pathnya, "%s", path);
	caesar_encrypt(pathnya);

  	char fpath[1000];
	sprintf(fpath, "%s%s",dirpath,pathnya);

	int res = 0;
  	int fd = 0 ;

	(void) fi;
	fd = open(fpath, O_RDONLY);
	if (fd == -1)
		return -errno;

	res = pread(fd, buf, size, offset);
	if (res == -1)
		res = -errno;
	close(fd);
	return res;
}

static int xmp_write(const char *path, const char *buf, size_t size,
		     off_t offset, struct fuse_file_info *fi)
{
	char pathnya[100];
	sprintf(pathnya, "%s", path);
	caesar_encrypt(pathnya);

	int fd;
	int res;

  	char fpath[1000];
  	sprintf(fpath,"%s%s",dirpath,pathnya);

	(void) fi;
	fd = open(fpath, O_WRONLY);
	if (fd == -1)
		return -errno;

	res = pwrite(fd, buf, size, offset);
	if (res == -1)
		res = -errno;

	close(fd);
	return res;
}

static struct fuse_operations xmp_oper = {
	.getattr	= xmp_getattr,
  	.access		= xmp_access,
	.readdir	= xmp_readdir,
	.read		= xmp_read,
  	.write 		= xmp_write,
};

int main(int argc, char *argv[])
{
	umask(0);
	return fuse_main(argc, argv, &xmp_oper, NULL);
}
