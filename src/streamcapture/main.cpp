
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/file.h>
#include <string.h>
#include <pthread.h>
#include <linux/videodev2.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <errno.h>
#include <fcntl.h>
#include <time.h>
#include <sys/time.h>
#include <signal.h>
#include "v4l2uvc.h"
#include "avilib.h"

int main(int argc, char *argv[])   // -s 1280x720   -d  /dev/video1   -a  test.avi  -i 30
{
	FILE *file = NULL;
	struct vdIn *vd;
	unsigned char *tmpbuffer;
	char *filename = NULL;
	int isRecode=1;
	char *avifilename = NULL;
	
	char *sizestring = NULL;
	char *fpsstring  = NULL;
	char *separateur = NULL;
	int width ; 
	int height;
	int fps;
	int i;
		
	printf("No ffff %d\n" ,argc);
	if (1 == argc)
	{
		printf("No for for \n");
		
		width = 1920 ; 
		height = 1080 ;
		fps   = 30 ;
		filename = "/dev/video0";
		if(isRecode)avifilename = "test.avi";
	}
					
	int format = V4L2_PIX_FMT_MJPEG;
	int ret;
	int grabmethod = 1;
		
	file = fopen(filename, "wb");
	if(file == NULL) 
	{
		printf("Unable to open file for raw frame capturing\n ");		
		exit(1);
	}
	
	//v4l2 init
	vd = (struct vdIn *) calloc(1, sizeof(struct vdIn));
	if(init_videoIn(vd, (char *) filename, width, height,fps,format,grabmethod,avifilename) < 0)
	{
		exit(1);
	}
	
	if (video_enable(vd))
	{
	   exit(1);
	}
	
	if(isRecode){
		vd->avifile = AVI_open_output_file(vd->avifilename);
		if (vd->avifile == NULL ) 
		{
			printf("Error opening avifile %s\n",vd->avifilename);
			exit(1);
		}
		
		AVI_set_video(vd->avifile, vd->width, vd->height, fps, "MJPG");
		printf("recording to %s\n",vd->avifilename);
	}

	while(1)
	{		
		memset(&vd->buf, 0, sizeof(struct v4l2_buffer));
		vd->buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
		vd->buf.memory = V4L2_MEMORY_MMAP;
		ret = ioctl(vd->fd, VIDIOC_DQBUF, &vd->buf);
		if (ret < 0) 
		{
			printf("Unable to dequeue buffer");
			exit(1);
		}
		
		memcpy(vd->tmpbuffer, vd->mem[vd->buf.index],vd->buf.bytesused);
   
   		if(vd->framecount==100){
      		printf("w=%d h=%d frameCount=%d\n", vd->width , vd->height , vd->framecount);
			FILE *imgFile = NULL;
			char temp[100];
			sprintf(temp, "test_%d.jpg", vd->framecount);
			imgFile = fopen(temp, "wb");
			fwrite(vd->mem[vd->buf.index], vd->buf.bytesused, 1, imgFile);
			fclose(imgFile);
		}
	
		if(isRecode)AVI_write_frame(vd->avifile, (char*)vd->tmpbuffer,vd->buf.bytesused, vd->framecount);
	
		vd->framecount++;
		
		ret = ioctl(vd->fd, VIDIOC_QBUF, &vd->buf);
		if (ret < 0) 
		{
			printf("Unable to requeue buffer");
			exit(1);
		}
	}
	fclose(file);
	close_v4l2(vd);
}
