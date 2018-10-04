const int maxl = 12000;
 
int  n;
 
struct Line {
    char text[4096];
    int  len;
    int  pp, nn;
};
 
int nl;
Line line[maxl];
 
int  r, c;
 
int  b;
char buf[4096];
 
inline int zax(int a, int b) {
    return a > b ? a : b;
}
 
inline int zin(int a, int b) {
    return a < b ? a : b;
}
 
void done() {
    char *p = line[r].text;
    int  &l = line[r].len;
     
    for (register int i = l - 1; i >= c; --i)
        p[i + b] = p[i];
 
    for (register int i = 0; i < b; ++i)
        p[i + c] = buf[i];
 
    l += b; c += b;
    b = 0;
}
 
void init(void) {
    nl = 2;
    line[0].pp = line[0].nn = 1;
    line[1].pp = line[1].nn = 0;
    line[1].len = 0;
 
    r = 1;
    c = 0;
    b = 0;
}
 
void put_char(char c) {
    buf[b++] = c;
}
 
void backspace(void) {
    if (b) {
        --b;
        return;
    }
     
    if (c > 0) {
        char *p = line[r].text;
        int  &l = line[r].len;
        for (register int i = c; i < l; ++i)
            p[i - 1] = p[i];
        --l; --c;
    } else if (line[r].pp != 0) {
        char *p1 = line[line[r].pp].text;
        int  &l1 = line[line[r].pp].len;
        char *p2 = line[r].text;
        int  &l2 = line[r].len;
        for (register int i = 0; i < l2; ++i)
            p1[l1 + i] = p2[i];
        c = l1; l1 += l2;
         
        line[line[r].pp].nn = line[r].nn;
        line[line[r].nn].pp = line[r].pp;
        r = line[r].pp;
    }
}
 
void put_enter(void) {
    if (b) done();
     
    line[nl].nn = line[r].nn;
    line[nl].pp = r;
    line[line[r].nn].pp = nl;
    line[r].nn = nl;
     
    char *p1 = line[r].text;
    int  &l1 = line[r].len;
    char *p2 = line[nl].text;
    int  &l2 = line[nl].len;
     
    for (register int i = c; i < l1; ++i)
        p2[i - c] = p1[i];
     
    l2 = l1 - c;
    l1 = c;
     
    r = nl++;
    c = 0;
}
 
void move_cursor_up(int k) {
    if (b) done();
     
    while (k-- && line[r].pp)
        r = line[r].pp;
 
    c = zin(c, line[r].len);
}
 
void move_cursor_down(int k) {
    if (b) done();
     
    while (k-- && line[r].nn)
        r = line[r].nn;
     
    c = zin(c, line[r].len);
}
 
void move_cursor_left(int k) {
    if (b) done();
    c = zax(c - k, 0);
}
 
void move_cursor_right(int k) {
    if (b) done();
    c = zin(c + k, line[r].len);
}
 
void close(register char* d) {
    if (b) done();
     
    int i = line[0].nn;
    while (i) {
        register char *p = line[i].text;
        for (register int j = line[i].len; j; --j)
            *d++ = *p++;
        i = line[i].nn;
        if (i) *d++ = '\n';
    }
}
