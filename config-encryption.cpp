// config-encryption.cpp: 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "string.h"
#include <stdlib.h>
#include <locale.h>
#include <ctype.h>

const char seed[32] = "87198565048417124016948602551546"; //32bit number

static const char usage_message[] =
" USAGE:\n"
"    config-encryption [/c|/d] [src_file] [out_file|stdout] [pass] \n\n"
" Options:\n"
"    /? Display this help message\n"
"    /c Encryption mode\n"
//"    /d Decryption mode\n\n"
" Examples:\n"
"    > config-encryption -c src.conf out.dat password\n"
"    > config-encryption -c src.conf stdout password\n"
//"    > config-encryption -d src.dat out.dat password\n"
//"    > config-encryption -d src.dat stdout password\n"
"\n"
;

void crpypt(char file_path[], char crpypt_path[], char fp_password[]) {
	int ch, i = 0, pw_len = strlen(fp_password);
	FILE *source, *crpypt_test, *crpypt_fp;
	char *sign = new char[8];
	errno_t err;

	err = fopen_s(&source, file_path, "r");
	if (err == 0) {
		printf("源文件成功打开\n");
	}
	else {
		printf("源文件打开失败\n");
		return;
	}

	if (strcmp(crpypt_path, "stdout") == 0) //判断文件名是否为stdout
	{
		crpypt_fp = stdout;//fb绑定为终端输入
	}
	else {
		err = fopen_s(&crpypt_test, crpypt_path, "rb");
		if (err == 0) {
			if (crpypt_test != NULL)fclose(crpypt_test);
			err = fopen_s(&crpypt_fp, crpypt_path, "w");
			if (err == 0) {
				printf("加密文件成功打开\n");
			}
		}
		else {
			if (crpypt_test != NULL)fclose(crpypt_test);
			err = fopen_s(&crpypt_fp, crpypt_path, "a");
			if (err == 0) {
				printf("加密文件成功创建\n");
			}
			else {
				printf("未知错误\n");
				printf(usage_message);
				return;
			}
		}
	}

	while ((ch = fgetc(source)) != EOF) {
		*sign = fp_password[i % pw_len];
		fputwc(ch ^ atoi(sign), crpypt_fp);
		i++;
	}

	delete sign;

	if (crpypt_fp != stdout)fclose(crpypt_fp);
	fclose(source);
	printf("\n");
	printf("加密完成");
}

void decrypt(char file_path[], char crpypt_path[], char fp_password[]) {
	int ch, i = 0, pw_len = strlen(fp_password);
	FILE *source, *crpypt_test, *crpypt_fp;
	char *sign = new char[8];
	errno_t err;

	err = fopen_s(&source, file_path, "r");
	if (err == 0) {
		printf("源文件成功打开\n");
	}
	else {
		printf("源文件打开失败\n");
		return;
	}

	if (strcmp(crpypt_path, "stdout") == 0) //判断文件名是否为stdout
	{
		crpypt_fp = stdout;//fb绑定为终端输入
	}
	else {
		err = fopen_s(&crpypt_test, crpypt_path, "r");
		if (err == 0) {
			if (crpypt_test != NULL)fclose(crpypt_test);
			err = fopen_s(&crpypt_fp, crpypt_path, "w");
			if (err == 0) {
				printf("解密文件成功打开\n");
			}
		}
		else {
			if (crpypt_test != NULL)fclose(crpypt_test);
			err = fopen_s(&crpypt_fp, crpypt_path, "a");
			if (err == 0) {
				printf("解密文件成功创建\n");
			}
			else {
				printf("未知错误\n");
				printf(usage_message);
				return;
			}
		}
	}

	while ((ch = fgetc(source)) != EOF) {
		*sign = fp_password[i % pw_len];
		fputwc(ch ^ atoi(sign), crpypt_fp);
		i++;
	}

	delete sign;

	if (crpypt_fp != stdout)fclose(crpypt_fp);
	fclose(source);
	printf("\n");
	printf("解密完成");
}

void strToHex(char *pbDest, const char *pbSrc, const char *pbSeed, const int nLen) {

	int i = 0;
	char ch[16];
	char *chs = new char[16];
	strcpy_s(pbDest, 2048, "");

	while (i <= nLen) {
		strcpy_s(chs, 4, "");
		sprintf_s(ch, 16, "%d", pbSrc[i % nLen]);
		if ((i & 1) == 0) {
			sprintf_s(chs, 16, "%c", pbSeed[i % strlen(pbSeed)]);
			_itoa_s(atoi(ch) * atoi(chs), chs, 8, 10);
		}
		else if ( 0 == i % 5 || 0 == i % 4 || 0 == i % 3){
			sprintf_s(chs, 16, "%d", pbSeed[i % strlen(pbSeed)]);
			_itoa_s(i % 16 * 2 * atoi(chs), chs, 8, 10);
		}
		else {
			sprintf_s(chs, 16, "%c", pbSeed[i % strlen(pbSeed)]);
			_itoa_s(i % 8 * atoi(ch) * atoi(chs), chs, 8, 10);
		}
		_itoa_s(atoi(ch)*atoi(chs),ch, 16,10);
		strcat_s(pbDest, 2048, ch);
		i++;
	}
	i = 0;
	int pLen= strlen(pbDest);
	while (i <= pLen) {
		sprintf_s(ch, 16, "%d", pbSrc[i % nLen]);
		if (0 == i % 5) {
			sprintf_s(chs, 16, "%d", pbDest[i % strlen(pbDest)]);
			_itoa_s(atoi(ch) * 20481, chs, 16, 10);
		}
		else if (0 == i % 4) {
			sprintf_s(chs, 16, "%d", pbDest[i % strlen(pbDest)]);
			_itoa_s(atoi(ch) * atoi(chs) * 1024, chs, 16, 10);
		}
		else if (0 == i % 3) {
			sprintf_s(chs, 16, "%d", pbDest[i % strlen(pbDest)]);
			_itoa_s(i % atoi(ch) * atoi(chs) * 512, chs, 16, 10);
		}
		else if ((i & 1) == 0) {
			sprintf_s(chs, 16, "%c", pbDest[i % strlen(pbDest)]);
			_itoa_s(i % 16 * atoi(ch) *atoi(chs), chs, 16, 10);
		}
		else {
			sprintf_s(chs, 16, "%c", pbDest[i % strlen(pbDest)]);
			_itoa_s(i % atoi(ch) * atoi(chs) * 8, chs, 16, 10);
		}
		_itoa_s(i % 16 * atoi(ch) * atoi(chs), ch, 16, 10);
		if (atoi(ch) < 0)_itoa_s(atoi(ch)*-1, ch, 16, 10);
		strcat_s(pbDest, 2048, ch);
		i++;
	}

	delete chs;
}



int main(int argc, char *argv[])
{
	if (argc < 5 || strcmp(argv[1], "/c") && strcmp(argv[1], "/d")) {
		printf(usage_message);
		return 4;
	}
	if (strlen(argv[4]) > 42 || strlen(argv[4]) < 8) {
		printf("Password length cannot exceed 42 and should not be less than 8.\n");
		printf(usage_message);
		return 4;
	}
	char *source_path = argv[2],
		 *crpypt_path = argv[3],
		 *password = new char[2048];
	strToHex(password, argv[4], seed, strlen(argv[4]));
	strlen(password);
	if (strcmp(argv[1], "/c") == 0) {
		crpypt(source_path, crpypt_path, password);
	}
	else if (strcmp(argv[1], "/d") == 0) {
		decrypt(source_path, crpypt_path, password);
	}
	delete password;
	getchar();
	return 0;
}
