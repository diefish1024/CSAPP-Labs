#include "cachelab.h"
#include <unistd.h>
#include <getopt.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <limits.h>

typedef struct cache_line {
    int valid;
    int tag;
    int lru_cnt;
} Cache_line;

typedef struct cache_ {
    int S;
    int E;
    int B;
    Cache_line** line;
} Cache;

int hit_cnt, miss_cnt, eviction_cnt;
int verbose = 0;
char t[1000];
Cache *cache = NULL;

void init_cache(int s, int E, int b) {
    int S = 1 << s;
    int B = 1 << b;
    cache = ((Cache*)malloc(sizeof(Cache)));
    cache->S = S;
    cache->E = E;
    cache->B = B;
    cache->line = (Cache_line**)malloc(sizeof(Cache_line*) * S);
    for(int i = 0; i < S; ++i) {
        cache->line[i] = (Cache_line*)malloc(sizeof(Cache_line) * E);
        for(int j = 0; j < E; ++j) {
            cache->line[i][j].valid = 0;
            cache->line[i][j].tag = -1;
            cache->line[i][j].lru_cnt = 0;
        }
    }
}

int cur_stamp = 0;

void upd(int op_tag, int op_s){
    int E = cache->E;
    for(int i = 0; i < E; ++i) {
        if(cache->line[op_s][i].tag == op_tag){
            if(verbose) {
                printf("hit");

            }
            hit_cnt++;
            cache->line[op_s][i].lru_cnt = ++cur_stamp;
            return;
        }
    }
    for(int i = 0; i < E; ++i) {
        if(cache->line[op_s][i].valid == 0) {
            if(verbose) {
                printf("miss");
            }
            miss_cnt++;
            cache->line[op_s][i].tag = op_tag;
            cache->line[op_s][i].valid = 1;
            cache->line[op_s][i].lru_cnt = ++cur_stamp;
            return;
        }
    }
    int min_lru = 0, min_id;
    for(int i = 0; i < E; ++i) {
        if(min_lru < cache->line[op_s][i].lru_cnt){
            min_lru = cache->line[op_s][i].lru_cnt;
            min_id = i;
        }
    }
    if(verbose) {
        printf("eviction");
    }
    miss_cnt++;
    eviction_cnt++;
    cache->line[op_s][min_id].tag = op_tag;
    cache->line[op_s][min_id].lru_cnt = ++cur_stamp;
}

void free_cache() {
    int S = cache->S;
    for(int i = 0;  i < S; ++i){
        free(cache->line[i]);
    }
    free(cache->line);
    free(cache);
}

void get_trace(int s, int E, int b) {
    FILE *pFile;
    pFile = fopen(t, "r");
    if(pFile == NULL){
        exit(-1);
    }
    char identifier;
    unsigned address;
    int size;
    while(fscanf(pFile, " %c %x, %d", &identifier, &address, &size) > 0) {
        int op_tag = address >> (s+b);
        int op_s = (address >> b) & ((unsigned)(-1) >> (8 * sizeof(unsigned) - s));
        switch (identifier) {
        case 'M':
            upd(op_tag, op_s);
            upd(op_tag, op_s);
            break;
        case 'L':
            upd(op_tag, op_s);
            break;
        case 'S':
            upd(op_tag, op_s);
            break;
        }
    }
    fclose(pFile);
}

void print_help()
{
    printf("** A Cache Simulator by Deconx\n");
    printf("Usage: ./csim-ref [-hv] -s <num> -E <num> -b <num> -t <file>\n");
    printf("Options:\n");
    printf("-h         Print this help message.\n");
    printf("-v         Optional verbose flag.\n");
    printf("-s <num>   Number of set index bits.\n");
    printf("-E <num>   Number of lines per set.\n");
    printf("-b <num>   Number of block offset bits.\n");
    printf("-t <file>  Trace file.\n\n\n");
    printf("Examples:\n");
    printf("linux>  ./csim -s 4 -E 1 -b 4 -t traces/yi.trace\n");
    printf("linux>  ./csim -v -s 8 -E 2 -b 4 -t traces/yi.trace\n");
}
int main(int argc, char *argv[]) {
    char opt;
    int s, E, b;
    while (-1 != (opt = getopt(argc, argv, "hvs:E:b:t:")))
    {
        switch (opt)
        {
        case 'h':
            print_help();
            exit(0);
        case 'v':
            verbose = 1;
            break;
        case 's':
            s = atoi(optarg);
            break;
        case 'E':
            E = atoi(optarg);
            break;
        case 'b':
            b = atoi(optarg);
            break;
        case 't':
            strcpy(t, optarg);
            break;
        default:
            print_help();
            exit(-1);
        }
    }
    init_cache(s, E, b);
    get_trace(s, E, b);
    free_cache();
    printSummary(hit_cnt, miss_cnt, eviction_cnt);
    return 0;
}