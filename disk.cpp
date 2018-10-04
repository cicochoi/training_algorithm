extern bool change_disk(int disk);
extern bool read_disk(int sector, char* data);
extern bool write_disk(int sector, char* data);
 
int mm;
int mlen[16384];
 
int pp[16384], nn[16384];
 
int mq;
int mque[256];
 
int id[100];
 
char data_t[1024], dirty[1024];
     
static inline void diskread(int p, char *data) {
    int d = p / 1024;
    int s = p % 1024;
 
    change_disk(d);
    read_disk(s, data);
}
 
static void diskwrite(int p, char *data) {
    int d = p / 1024;
    int s = p % 1024;
 
    change_disk(d);
    write_disk(s, data);
}
 
static inline int gethash(char *filename) {
    int ret = 0;
    for (int c = 0; c < 6; ++c)
        ret += ret * 26 + (filename[c] - 'A');
    return ret;
}
 
static inline int getfileid(char *filename) {
    int fid = gethash(filename);
     
    int p = 0;
    while(id[p]) {
        if (id[p] == fid)
            return p;
        ++p;
    }
     
    id[p] = fid;
    return p;
}
 
static inline int getnode() {
    return mq > 0 ? mque[--mq] : mm++;
}
 
static inline void freenode(int m) {
    mque[mq++] = m;
}
 
void init() {
    for (int i = 0; i < 100; ++i)
        id[i] = 0;
 
    mm = 100;   
    for (int i = 0; i < 100; ++i)
        pp[i] = nn[i] = i;
     
    mq = 0;
}
 
void read_file(char* filename, int offset, char* data, int size) {
    int p = getfileid(filename);
 
    register int q, r, o;
 
    q = nn[p];
    while (1) {
        if (offset < mlen[q])
            break;
        offset -= mlen[q];
        q = nn[q];
    }
 
    r = offset; o = 0;
     
    diskread(q, data_t);
     
    while (o < size && r < mlen[q])
        data[o++] = data_t[r++];
     
    q = nn[q];
 
    if (size <= 3072) {
        while (o < size) {
            diskread(q, data + o);
            o += mlen[q];
            q = nn[q];
        }
    } else {
        while (o <= 3072) {
            diskread(q, data + o);
            o += mlen[q];
            q = nn[q];
        }
        while (o < size) {
            diskread(q, data_t);
            r = 0;
            while (o < size && r < mlen[q])
                data[o++] = data_t[r++];
            q = nn[q];
        }
    }
}
 
int cnt = 0;
 
void insert_file(char* filename, int offset, char* data, int size) {
    int p = getfileid(filename);
     
    register int o, q, r;
    int s, mid;
     
    q = nn[p];
    while (q != p) {
        if (offset < mlen[q])
            break;
        offset -= mlen[q];
        q = nn[q];
    }
 
    r = 0;
    if (q != p) {
        diskread(q, dirty);
        while(r < offset) {
            data_t[r] = dirty[r];
            ++r;
        }
        s = r;
        o = 0;
        while(r < 1024 && o < size)
            data_t[r++] = data[o++];
         
        mid = getnode();
        nn[mid] = q;
        pp[mid] = pp[q];
        nn[pp[q]] = mid;
        pp[q] = mid;
 
        diskwrite(mid, data_t);
        mlen[mid] = r;
     
        while(o + 1024 <= size) {
            mid = getnode();
            nn[mid] = q;
            pp[mid] = pp[q];
            nn[pp[q]] = mid;
            pp[q] = mid;
            diskwrite(mid, data + o);
            o += 1024;
            mlen[mid] = 1024;
        }
 
        r = 0;
        while (o < size)
            data_t[r++] = data[o++];
 
        o = s;
        do {
            while(o < mlen[q] && r < 1024)
                data_t[r++] = dirty[o++];
            mid = getnode();
            nn[mid] = q;
            pp[mid] = pp[q];
            nn[pp[q]] = mid;
            pp[q] = mid;    
            diskwrite(mid, data_t);
            mlen[mid] = r;          
            r = 0;
        } while(o < mlen[q]);
 
        nn[pp[q]] = nn[q];
        pp[nn[q]] = pp[q];
        freenode(q);
    } else {
        o = 0;
        while(o < size) {
            mid = getnode();
            nn[mid] = q;
            pp[mid] = pp[q];
            nn[pp[q]] = mid;
            pp[q] = mid;
            r = 0;
            diskwrite(mid, data + o);
            mlen[mid] = o + 1024 < size ? 1024 : size - o;
            o += 1024;
        }
    }
}
 
void delete_file(char* filename, int offset, int size) {
    int p = getfileid(filename);
     
    register int o, q, r;
             
    q = nn[p];
    while (q != p) {
        if (offset < mlen[q])
            break;
        offset -= mlen[q];
        q = nn[q];
    }
     
    if (size <= mlen[q] - offset) {
        diskread(q, data_t);
        for (int i = offset + size; i < mlen[q]; ++i)
            data_t[i - size] = data_t[i];
        diskwrite(q, data_t);
        mlen[q] -= size;;
         
        if (mlen[q] == 0) {
            nn[pp[q]] = nn[q];
            pp[nn[q]] = pp[q];
            freenode(q);
            q = nn[q];
        }
    } else {
        size -= mlen[q] - offset;
        mlen[q] = offset;
        q = nn[q];
     
        while (size > 0 && size >= mlen[q]) {
            size -= mlen[q];
            nn[pp[q]] = nn[q];
            pp[nn[q]] = pp[q];
            freenode(q);
            q = nn[q];
        }
 
        if (size > 0) {
            diskread(q, data_t);
            for (int i = size; i < mlen[q]; ++i)
                data_t[i - size] = data_t[i];
            diskwrite(q, data_t);
            mlen[q] -= size;
        }
    }
}
