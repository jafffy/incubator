#ifndef LIBCAMERA_H_
#define LIBCAMERA_H_

struct camera_context
{
	int screen_width,screen_height;//size of image
	int lcd_width,lcd_height;//size of lcd 
	int bits_per_pixel;
	int line_length;
	int mem_size;	

	int m_cam_fd;
	unsigned char* fb_mem;

};

static int close_buffers(struct SecBuffer *buffers, int num_of_buf);
static int fimc_poll(struct pollfd *events);
static int fimc_v4l2_querycap(int fp);
static const __u8* fimc_v4l2_enuminput(int fp, int index);
static int fimc_v4l2_s_input(int fp, int index);
static int fimc_v4l2_s_fmt(int fp, int width, int height, unsigned int fmt, enum v4l2_field field, unsigned int num_plane);
static int fimc_v4l2_s_fmt_cap(int fp, int width, int height, unsigned int fmt);
int fimc_v4l2_s_fmt_is(int fp, int width, int height, unsigned int fmt, enum v4l2_field field);
static int fimc_v4l2_enum_fmt(int fp, unsigned int fmt);
static int fimc_v4l2_reqbufs(int fp, enum v4l2_buf_type type, int nr_bufs);
static int fimc_v4l2_querybuf(int fp, struct SecBuffer *buffers, enum v4l2_buf_type type, int nr_frames, int num_plane);
static int fimc_v4l2_streamon(int fp);
static int fimc_v4l2_streamoff(int fp);
static int fimc_v4l2_qbuf(int fp, int index);
static int fimc_v4l2_dqbuf(int fp, int num_plane);
static int fimc_v4l2_g_ctrl(int fp, unsigned int id);
static int fimc_v4l2_s_ctrl(int fp, unsigned int id, unsigned int value);
static int fimc_v4l2_s_ext_ctrl(int fp, unsigned int id, void *value);
static int fimc_v4l2_g_parm(int fp, struct v4l2_streamparm *streamparm);
static int fimc_v4l2_s_parm(int fp, struct v4l2_streamparm *streamparm);
static void DrawFromRGB565(unsigned char *displayFrame, unsigned char *videoFrame, int sx, int sy, int videoWidth, int videoHeight, \
				 int dFrameWidth,int dFrameHeight);
int CreateCamera(int index);//카메라 드라이버 등록?
void DestroyCamera();//카메라 드라이버 해제
void startPreview();
int stopPreview();
void initScreen();//스크린 검은색으로 클리어
void initCamera();//카메라 설정
void drawCameraImage(int sx, int sy);//x,y좌표 주면 거기서 부터 이미지그림



#endif

















