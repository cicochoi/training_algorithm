#include <stdio.h>
#include <memory.h>
#include <time.h>

#define SIZE 4096

extern void recover(unsigned char map[SIZE][SIZE]);

static int seed = 1124; 
static unsigned char org_map[SIZE][SIZE];
static unsigned char new_map[SIZE][SIZE];
static unsigned char xoverlay[SIZE][SIZE];
static unsigned char yoverlay[SIZE][SIZE];
static unsigned char dummy1[256]; 
static unsigned char dummy2[SIZE];

bool check_map(unsigned char map[SIZE][SIZE]){
	if (memcmp(org_map, map, SIZE*SIZE)) return false;
	return true;
}

static int pseudo_rand(void){
	seed = seed * 214013 + 2531011;
	return (seed >> 16) & 0x7FFF;
}

static bool check_line(int x1, int y1, int x2, int y2)
{
	if (x2 - x1 < 16) return false;
	if (y2 - y1 < 16) return false;
	if (memcmp(&xoverlay[y1][x1], dummy2, x2 - x1 + 1)) return false;
	if (memcmp(&xoverlay[y2][x1], dummy2, x2 - x1 + 1)) return false;
	if (memcmp(&yoverlay[x1][y1], dummy2, y2 - y1 + 1)) return false;
	if (memcmp(&yoverlay[x2][y1], dummy2, y2 - y1 + 1)) return false;
	return true;
}

static void draw_rect(void)
{
	register int x1;
	register int y1;
	register int x2;
	register int y2;

	while (true)	{
		x1 = 16 + (pseudo_rand() % 4064);
		y1 = 16 + (pseudo_rand() % 4064);
		x2 = 16 + (pseudo_rand() % 4064);
		y2 = 16 + (pseudo_rand() % 4064);

		if (x1 > x2){
			int tmp = x1;
			x1 = x2;
			x2 = tmp;
		}

		if (y1 > y2){
			int tmp = y1;
			y1 = y2;
			y2 = tmp;
		}

		if (check_line(x1, y1, x2, y2))
		{
			memset(&xoverlay[y1][x1], 1, x2 - x1 + 1);
			memset(&xoverlay[y2][x1], 1, x2 - x1 + 1);
			memset(&yoverlay[x1][y1], 1, y2 - y1 + 1);
			memset(&yoverlay[x2][y1], 1, y2 - y1 + 1);
			break;
		}
	}

	register int c = pseudo_rand() % 255 + 1;
	for (register int x = x1; x <= x2; x++) {
		org_map[y1][x] = c;
		org_map[y2][x] = c;
	}
	for (register int y = y1; y <= y2; y++) {
		org_map[y][x1] = c;
		org_map[y][x2] = c;
	}
}

static void mixing_map() {
	for (register int c = 0; c < 256; c++)
	{
		register int x1 = pseudo_rand() % 16;
		register int y1 = pseudo_rand() % 16;
		register int x2 = pseudo_rand() % 16;
		register int y2 = pseudo_rand() % 16;

		for (register int cy = 0; cy < 256; cy++) {
			memcpy(dummy1, &new_map[y1 * 256 + cy][x1 * 256], 256);
			memcpy(&new_map[y1 * 256 + cy][x1 * 256], &new_map[y2 * 256 + cy][x2 * 256], 256);
			memcpy(&new_map[y2 * 256 + cy][x2 * 256], dummy1, 256);
		}
	}
}

static void create_map(void)
{
	for (register int y = 0; y < SIZE; y++)
		for (register int x = 0; x < SIZE; x++) {
			org_map[y][x] = 0;
			xoverlay[y][x] = 0;
			yoverlay[y][x] = 0;
		}
	
	for (int c = 0; c < 1024; c++)
		draw_rect();

	for (register int y = 0; y < SIZE; y++)
		for (register int x = 0; x < SIZE; x++) 
			new_map[y][x] = org_map[y][x];

	mixing_map();	
}



int main(void)
{
	int N;
	scanf("%d", &N); // no me
  aning, but at least one input is required to the marking system.

	setbuf(stdout, NULL);
	int score = 1000000;
	memset(dummy2, 0, SIZE);
	for (int T = 0; T < 10; T++){
		create_map();
		time_t START = clock();
		recover(new_map);
		score += (int)((clock() - START) / (CLOCKS_PER_SEC / 1000));
		if (check_map(new_map))
			score -= 100000;
	}
	if ( score <= 3000 )
		printf("PASS\n");
	else
		printf("FAIL\n");
	printf("SCORE : %d\n", score);

	return 0;
}



extern bool check_map(unsigned char map[4096][4096]);
 
unsigned char cell[256][256][256];
 
struct Block {
    int eg[4][256];
 
    int nn[4];
    int ne[4][256];
    int ha[4];
     
    bool used;
};
 
int   nb;
Block block[256];
 
int fin[16][16];
int seq[4][256];
 
const int o[4] = { 2, 3, 0, 1 };
 
inline int zin(int a, int b) {
    return a < b ? a : b;
}
 
inline int zax(int a, int b) {
    return a > b ? a : b;
}
 
int calchash(int b[]) {
    int ret = 0;
     
    for (int k = 0; k < 226; ++k) {
        ret = *b++ + (ret << 6) + (ret << 16) - ret;
        ret &= 0x7fffffff;
    }
 
    return ret; 
}
 
int find(int b, int arr[], int k, int h) {
    int e = nb - 1;
    if (block[arr[e]].ha[k] < h) return -1;
     
    while(b < e) {
        int m = (b + e) / 2;
        if (block[arr[m]].ha[k] < h) {
            b = m + 1;
        } else {
            e = m;
        }
    }
     
    if (block[arr[b]].ha[k] == h)
        return b;
     
    return -1;
}
 
int ext[256];
 
inline void zwap(int &a, int &b) {
    int t = a; a = b; b = t;
}
 
void mergesort(int arr[], int b, int e, int k) {
    if (b == e) return;
    if (e - b == 1) {
        if (block[arr[b]].ha[k] > block[arr[e]].ha[k])
            zwap(arr[b], arr[e]);
        return;
    }
     
    int m = (b + e) / 2;
     
    mergesort(arr, b, m, k);
    mergesort(arr, m + 1, e, k);
     
    for (int l = b; l <= e; ++l) ext[l] = arr[l];
     
    int p0, p1, p2;
 
    p0 = p1 = b; p2 = m + 1;
    while (p1 <= m && p2 <= e) {
        if (block[ext[p1]].ha[k] < block[ext[p2]].ha[k])
            arr[p0++] = ext[p1++];
        else
            arr[p0++] = ext[p2++];
    }
     
    while (p1 <= m) arr[p0++] = ext[p1++];
    while (p2 <= e) arr[p0++] = ext[p2++];
}
 
int getblock(int k1, int k2, int h1, int h2) {
    int b = -1;
    while((b = find(b + 1, seq[k1], k1, h1)) != -1)
        if (block[seq[k1][b]].ha[k2] == h2)
            return seq[k1][b];
    return -1;
}
 
bool islinex1(unsigned char arr[256][256], int x1, int y1) {
    int x2 = zin(x1 + 10, 255);
    for (register int x3 = x1 + 1; x3 <= x2 && arr[y1][x3] != 0; ++x3)
        if (arr[y1][x3] == arr[y1][x1])
            return true;
    return false;
}
 
bool islinex2(unsigned char arr[256][256], int x1, int y1) {
    int x2 = zax(x1 - 10, 0);
    for (register int x3 = x1 - 1; x3 >= x2 && arr[y1][x3] != 0; --x3)
        if (arr[y1][x3] == arr[y1][x1])
            return true;
    return false;
}
 
bool isliney1(unsigned char arr[256][256], int x1, int y1) {
    int y2 = zin(y1 + 10, 255);
    for (register int y3 = y1 + 1; y3 <= y2 && arr[y3][x1] != 0; ++y3)
        if (arr[y3][x1] == arr[y1][x1])
            return true;
    return false;
}
 
bool isliney2(unsigned char arr[256][256], int x1, int y1) {
    int y2 = zax(y1 - 10, 0);
    for (register int y3 = y1 - 1; y3 >= y2 && arr[y3][x1] != 0; --y3)
        if (arr[y3][x1] == arr[y1][x1])
            return true;
    return false;
}
 
void prep0(int e[256], unsigned char arr[256][256]) {   
    for (int x1 = 0; x1 < 256; ++x1) {
        if (arr[0][x1] > 0) {
            bool bx1 = islinex1(arr, x1, 0);
            bool bx2 = islinex2(arr, x1, 0);
            if (bx1 && bx2) {
                int y1;
                for (y1 = 1; y1 < 256 && arr[y1][x1] != 0; ++y1) {
                    if (!islinex1(arr, x1, y1) && !islinex2(arr, x1, y1))
                        break;
                }
                e[x1] = y1 < 256 ? arr[y1][x1] : 0;
            } else if ((bx1 || bx2) && isliney1(arr, x1, 0)) {
                e[x1] = 0;
            } else
                e[x1] = arr[0][x1];
        } else {
            e[x1] = 0;
        }
    }
}
 
void prep1(int e[256], unsigned char arr[256][256]) {
    for (int y1 = 0; y1 < 256; ++y1) {
        if (arr[y1][255] > 0) {
            bool by1 = isliney1(arr, 255, y1);
            bool by2 = isliney2(arr, 255, y1);
            if (by1 && by2) {
                int x1;
                for (x1 = 254; x1 >= 0 && arr[y1][x1] != 0; --x1) {
                    if (!isliney1(arr, x1, y1) && !isliney2(arr, x1, y1))
                        break;
                }
                e[y1] = x1 >= 0 ? arr[y1][x1] : 0;
            } else if ((by1 || by2) && islinex1(arr, 255, y1)) {
                e[y1] = 0;
            } else
                e[y1] = arr[y1][255];
        } else {
            e[y1] = 0;
        }
    }   
}
             
         
void prep2(int e[256], unsigned char arr[256][256]) {       
    for (int x1 = 0; x1 < 256; ++x1) {
        if (arr[255][x1] > 0) {
            bool bx1 = islinex1(arr, x1, 255);
            bool bx2 = islinex2(arr, x1, 255);
            if (bx1 && bx2) {
                int y1;
                for (y1 = 254; y1 >= 0 && arr[y1][x1] != 0; --y1) {
                    if (!islinex1(arr, x1, y1) && !islinex2(arr, x1, y1))
                        break;
                }
                e[x1] = y1 >= 0 ? arr[y1][x1] : 0;
            } else if ((bx1 || bx2) && isliney1(arr, x1, 255)) {
                e[x1] = 0;
            } else
                e[x1] = arr[255][x1];
        } else {
            e[x1] = 0;
        }
    }
}
 
void prep3(int e[256], unsigned char arr[256][256]) {
    for (int y1 = 0; y1 < 256; ++y1) {
        if (arr[y1][0] > 0) {
            bool by1 = isliney1(arr, 0, y1);
            bool by2 = isliney2(arr, 0, y1);
            if (by1 && by2) {
                int x1;
                for (x1 = 1; x1 < 256 && arr[y1][x1] != 0; ++x1) {
                    if (!isliney1(arr, x1, y1) && !isliney2(arr, x1, y1))
                        break;
                }
                e[y1] = x1 < 256 ? arr[y1][x1] : 0;
            } else if ((by1 || by2) && islinex1(arr, 0, y1)) {
                e[y1] = 0;
            } else
                e[y1] = arr[y1][0];
        } else {
            e[y1] = 0;
        }
    }
}
 
inline int diff(int b1, int b2, int k1, int k2) {
    int ret = 0;
     
    for (register int i = 15; i < 241; ++i)
        if (block[b1].eg[k1][i] != block[b2].eg[k2][i])
            ++ret;
    return ret;
}
 
void recover(unsigned char map[4096][4096]) {   
    nb = 0;
    for (int y = 0; y < 16; ++y)
    for (int x = 0; x < 16; ++x) {
        register unsigned char *p = &cell[nb][0][0];
        register unsigned char *r = &map[y << 8][x << 8];
        for (register int dy = 0; dy < 256; ++dy) {
            for (register int dx = 0; dx < 256; ++dx)
                *p++ = *r++;
            r += 4096 - 256;
        }
        block[nb].nn[0] = block[nb].nn[1] = block[nb].nn[2] = block[nb].nn[3] = 0;
        block[nb].used = false;
        nb++;
    }
 
    for (int i = 0; i < nb; ++i) {
        prep0(block[i].eg[0], cell[i]);
        prep1(block[i].eg[1], cell[i]);
        prep2(block[i].eg[2], cell[i]);
        prep3(block[i].eg[3], cell[i]);
     
        for (int k = 0; k < 4; ++k)
            block[i].ha[k] = calchash(block[i].eg[k] + 15);
    }
     
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < nb; ++j)
            seq[i][j] = j;
        mergesort(seq[i], 0, nb - 1, i); 
    }
 
    for (int i = 0; i < nb; ++i) {
        for (int k = 0; k < 4; ++k) {
            int b = -1;
            while ((b = find(b + 1, seq[o[k]], o[k], block[i].ha[k])) != -1)
                block[i].ne[k][block[i].nn[k]++] = seq[o[k]][b];
        }
    }
 
    for (int i = 0; i < 16; ++i)
    for (int j = 0; j < 16; ++j)
        fin[i][j] = -1;
     
    fin[0][0] = getblock(0, 3, 0, 0);
    block[fin[0][0]].used = true;
    fin[0][15] = getblock(0, 1, 0, 0);
    block[fin[0][15]].used = true;
    fin[15][15] = getblock(2, 1, 0, 0);
    block[fin[15][15]].used = true;
    fin[15][0] = getblock(2, 3, 0, 0);
    block[fin[15][0]].used = true;
     
    int q = 0;
    int que[4096];
     
    que[q++] = 0; que[q++] = 0;
    que[q++] = 0; que[q++] = 15;
    que[q++] = 15; que[q++] = 0;
    que[q++] = 15; que[q++] = 15;
 
    int remained = 252;
     
    while(q > 0) {
        int y = que[--q];
        int x = que[--q];
        if (y > 0 && fin[y - 1][x] == -1 && block[fin[y][x]].nn[0] == 1) {
            fin[y - 1][x] = block[fin[y][x]].ne[0][0];
            block[fin[y - 1][x]].used = true;
            que[q++] = x;
            que[q++] = y - 1;
            --remained;
        }
        if (x < 15 && fin[y][x + 1] == -1 && block[fin[y][x]].nn[1] == 1) {
            fin[y][x + 1] = block[fin[y][x]].ne[1][0];
            block[fin[y][x + 1]].used = true;
            que[q++] = x + 1;
            que[q++] = y;
            --remained;
        }
        if (y < 15 && fin[y + 1][x] == -1 && block[fin[y][x]].nn[2] == 1) {
            fin[y + 1][x] = block[fin[y][x]].ne[2][0];
            block[fin[y + 1][x]].used = true;
            que[q++] = x;
            que[q++] = y + 1;
            --remained;
        }
        if (x > 0 && fin[y][x - 1] == -1 && block[fin[y][x]].nn[3] == 1) {
            fin[y][x - 1] = block[fin[y][x]].ne[3][0];
            block[fin[y][x - 1]].used = true;
            que[q++] = x -1;
            que[q++] = y;
            --remained;
        }
    }
     
    if (remained > 0) {
        int candi[256], nc = 0;
        for (int i = 0; i < nb; ++i)
            if (!block[i].used)
                candi[nc++] = i;
         
        for (int y = 0; y < 16; ++y)
        for (int x = 0; x < 16; ++x)
            if (fin[y][x] == -1) {
                int mm = 1000000, i0 = -1;
                for (int c = 0; c < nc; ++c) {
                    if (block[fin[y][x]].used == true) continue;
                    int sum = 0;
                    if (x > 0 && fin[y][x - 1] != -1)
                        sum += diff(fin[y][x - 1], candi[c], 1, 3);
                    if (x < 15 && fin[y][x + 1] != -1)
                        sum += diff(fin[y][x + 1], candi[c], 3, 1);
                    if (y > 0 && fin[y - 1][x] != -1)
                        sum += diff(fin[y - 1][x], candi[c], 2, 0);
                    if (y < 15 && fin[y + 1][x] != -1)
                        sum += diff(fin[y + 1][x], candi[c], 0, 2);
                    if (sum < mm) {
                        mm = sum;
                        i0 = candi[c];
                    }
                }
                fin[y][x] = i0;
                block[i0].used = true;
            }
    }
     
 
    for (int y = 0; y < 16; ++y)
    for (int x = 0; x < 16; ++x) {
        register unsigned char *p = &cell[fin[y][x]][0][0];
        register unsigned char *r = &map[y << 8][x << 8];
        for (register int dy = 0; dy < 256; ++dy) {
            for (register int dx = 0; dx < 256; ++dx) {
                *r++ = *p++;
            }
            r += 4096 - 256;
        }
    }
}
