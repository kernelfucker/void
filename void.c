/* void - minimal process viewer

   void.c

   written by kernelfucker
   
   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>. */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <dirent.h>

void getCpuUsage(){
	FILE *fp;
	double CpuUsage;

	fp = fopen("/proc/stat", "r");
	if (fp == NULL){
		perror("Failed to open /proc/stat");
	return;
}

char line[128];
fgets(line, 128, fp);
fclose(fp);

char* token = strtok(line, " ");
token = strtok(NULL, " ");
int user, last, system, idle;
sscanf(token, "%d", &user);
token = strtok(NULL, " ");
sscanf(token, "%d", &last);
token = strtok(NULL, " ");
sscanf(token, "%d", &system);
token = strtok(NULL, " ");
sscanf(token, "%d", &idle);

CpuUsage = ((double)(user + system + last) / (user + last + system + idle)) * 100;
printf("CPU Usage: %.2f%%\n", CpuUsage);
}

int ListProcesses(){
	DIR *dir;
	struct dirent *ent;
	char path[256];
	char name[256];
	FILE *fp;

	if ((dir = opendir("/proc")) != NULL){
		while ((ent = readdir(dir)) != NULL){
			if ((ent = readdir(dir)) !=NULL){
				if (atoi(ent->d_name) != 0){
					sprintf(path, "/proc/%s/comm",ent->d_name);
					fp = fopen(path, "r");
					if (fp != NULL){
						fgets(name, 256, fp);
						name[strcspn(name, "\n")] = 0;
						printf("PID: %s, Process: %s\n", ent->d_name, name);
						fclose(fp);
					} else{
						printf("PID: %s, Process: (unknown)\n", ent->d_name);
					}
				}
			}
		}
		closedir(dir);
	} else{
		perror("Failed to open /proc");
	}
}
int main(){
	while (1){
	system("clear");
	getCpuUsage();
	ListProcesses();
	sleep(1);
	}
	return 0;
}
