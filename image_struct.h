#pragma pack(1)

typedef struct {
	unsigned char r;
	unsigned char g;
	unsigned char b;
} RGB;

typedef struct {
	int type;
	int width;
	int height;
	int max_val;
} Image;

#pragma pack()