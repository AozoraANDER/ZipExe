// ZipInCode.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include <tchar.h>
#include <stdlib.h>

struct ZipData {
  char filename[256];
  int filesize;
};

int getFileSize(char* filename);

int _tmain(int argc, _TCHAR* argv[]) {
  // file count
  int num = argc - 1;

  // Prepare the info of file

  // struct ZipData info[num] = {0};

  struct ZipData* pInfo = (struct ZipData*)malloc(num * sizeof(struct ZipData));

  char compressedFileName[256];
  printf("Enter the name for the compressed output file (without extension): ");
  std::cin.getline(compressedFileName, 256);
  strcat_s(compressedFileName, ".AAfile");

  for (int i = 0; i < num; i++) {
    // get size of file, send file name and file size to the struct
    pInfo[i].filesize = getFileSize(argv[i + 1]);

    strcpy_s(pInfo[i].filename, 256, argv[i + 1]);

    printf("i:%d File Name: %s File Size: %d\n", i, pInfo[i].filename,
           pInfo[i].filesize);
  }

  // Create a new file
  FILE* fp;
  if (fopen_s(&fp, compressedFileName, "wb") != 0) {
    fprintf(stderr, "Failed to create output file: Compressed.AAfile\n");
    free(pInfo);
    return -1;
  }
  if (NULL == fp) return -1;
  // Write the count of file
  fwrite(&num, 1, 4, fp);
  // Write file information in
  fwrite(pInfo, 1, num * sizeof(struct ZipData), fp);
  // For loop read the information and write in

  FILE* fpSrc;
  int r;
  char buff[1024];

  for (int i = 0; i < num; i++) {
    if (fopen_s(&fpSrc, pInfo[i].filename, "rb") != 0) {
      fprintf(stderr, "Failed to open source file: %s\n", pInfo[i].filename);
      continue;
    }


    while (1) {
      r = fread_s(buff, sizeof(buff), 1, 1024, fpSrc);
      if (r > 0) {
        fwrite(buff, 1, r, fp);
      } else {
        break;
      }
    } 
    fclose(fpSrc);
  }
  while (1);
  return 0;
}

int getFileSize(char* filename){ 
    FILE* fp;
  fopen_s(&fp, filename, "r");
  if (NULL == fp) return -1;

  //file content pointer to the end
  fseek(fp, 0, SEEK_END);

  //file content pointer to the file size
  int size = ftell(fp);

  //file content pointer to the beginning
  fseek(fp, 0, SEEK_SET);
  fclose(fp);

  return size;
}