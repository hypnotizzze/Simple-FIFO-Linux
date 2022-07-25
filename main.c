#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>

#define FIFO "fifo"

int main() {
	FILE* file, * fifo;
	char filename[256];
	char buf[100];
	int str, stb, temp, flag = 0, i, j, k, sum;
	pid_t pid, parent_pid, ppid;

	if (mkfifo(FIFO, 0666) < 0) {
		printf("Ошибка создания %s\n", FIFO);
		return 1;
	}

	if ((fifo = fopen(FIFO, "r+")) == NULL) {
		printf("Не найден файл %s\n", FIFO);
		return 1;
	}

	while (flag == 0) {
		printf("Введите имя файла:");
		scanf("%s", filename);
		if ((file = fopen(filename, "r")) == NULL)
			printf("Не удалось открыть файл\n");
		else
			flag = 1;
	}
	parent_pid = getpid();
	pid = 0;
	fscanf(file, "%d", &str);
	fscanf(file, "%d", &stb);
	for (i = 0; i < str && pid == 0; i++) {
		pid = fork();
		switch (pid) {
		case -1: // Ошибка
			printf("Не удалось создать процесс\n");
			break;
		case 0: // Дочерний
			sum = 0;
			for (j = 0; j < stb; j++) {
				fscanf(file, "%d", &temp);
				sum += temp;
			}
			sprintf(buf, "%d ", sum);
			fputs(buf, fifo);
			break;
		default: // Родитель
			wait(NULL);
			break;
		}
	}
	pid = getpid();
	if (pid == parent_pid) {
		for (k = 0; k < str; k++) {
			fscanf(fifo, "%d", &temp);
			printf("Сумма в строке %d = %d\n", k + 1, temp);
		}
		unlink(FIFO);
	}
	fclose(file);
	return 0;
}

