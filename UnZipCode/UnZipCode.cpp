// UnZipCode.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include <tchar.h>
#include <stdlib.h>

struct ZipData {
  char filename[256];
  int filesize;
};

int _tmain(int argc, _TCHAR* argv[]) 
{ 
	//argv[1];
  FILE* fp;
  if (fopen_s(&fp, argv[1], "rb") != 0) {
    fprintf(stderr, "Failed to open file: %s\n", argv[1]);
    return -1;
  }

  int num;

  if (fread_s(&num, sizeof(num), sizeof(int), 1, fp) !=1) {  // Read total file count
    fprintf(stderr, "Failed to read file count\n");
    fclose(fp);
    return -1;
  }

  printf("%d\n", num);

  struct ZipData* pInfo = (struct ZipData*)malloc(num * sizeof(struct ZipData));

  if (pInfo == NULL) {
    fprintf(stderr, "Memory allocation failed\n");
    fclose(fp);
    return -1;
  }

  if (fread_s(pInfo, num * sizeof(struct ZipData), sizeof(struct ZipData), num,
              fp) != num) {
    fprintf(stderr, "Failed to read file metadata\n");
    free(pInfo);
    fclose(fp);
    return -1;
  }

  FILE* fpDst;
  char buff[1024];
  char c;
  int r;

  for (int i = 0; i < num; i++) {
    // Check if have file has same name.
    if (fopen_s(&fpDst, pInfo[i].filename, "r") == 0) {
      fclose(fpDst);  // if exist, close file.
      printf("File '%s' already exists. Overwrite? (y/n): ", pInfo[i].filename);
      char choice;
      std::cin >> choice;

      if (choice == 'n' || choice == 'N') {
        printf("Skipping file: %s\n", pInfo[i].filename);
        continue;  // skip it.
      }
    }
  }

  for (int i = 0; i < num; i++) {
  //Create file from the content one by one, read data from the packed file and write to new created file
    if (fopen_s(&fpDst, pInfo[i].filename, "wb") != 0) {
      fprintf(stderr, "Failed to create file: %s\n", pInfo[i].filename);
      continue;  // Skip this file if failed to open
    }

	for (int j = 0; j < pInfo[i].filesize; j++) {

      if (fread_s(&c, sizeof(c), 1, 1, fp) != 1) { 
          fprintf(stderr, "Failed to read content for file: %s\n", pInfo[i].filename);
        break;
          }
      if (fwrite(&c, 1, 1, fpDst) != 1) {
            fprintf(stderr, "Failed to write content to file: %s\n",
                pInfo[i].filename);
        break;
      }
	}
    fclose(fpDst);
    
  }

  fclose(fp);
  free(pInfo);

  printf("Unzip success\n");
  return 0;
}
