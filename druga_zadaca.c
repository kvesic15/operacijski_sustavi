#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/shm.h>

#define K 5

void proc(int i, int* ZASTAVICA, int* PRAVO) {
    int j = 1 - i, k, m;

    for (k = 1; k <= K; k++) {
        ZASTAVICA[i] = 1;
        while (ZASTAVICA[j] != 0) {
            if (*PRAVO == j) {
                ZASTAVICA[i] = 0;
                while (*PRAVO == j) {}
                ZASTAVICA[i] = 1;
            }
        }

        for (m = 1; m <= K; m++) {
            printf("Proces %d, k = %d, m = %d\n", i, k, m);
        }

        *PRAVO = j;
        ZASTAVICA[i] = 0;
    }
}

int main() {
    int shmid_flags = IPC_CREAT | 0666;

    int shmid_zastavica = shmget(IPC_PRIVATE, 2 * sizeof(int), shmid_flags);
    if (shmid_zastavica == -1) {
        perror("shmget ZASTAVICA");
        return 1;
    }

    int shmid_pravo = shmget(IPC_PRIVATE, sizeof(int), shmid_flags);
    if (shmid_pravo == -1) {
        perror("shmget PRAVO");
        return 1;
    }

    int* ZASTAVICA = (int*)shmat(shmid_zastavica, NULL, 0);
    int* PRAVO = (int*)shmat(shmid_pravo, NULL, 0);

    ZASTAVICA[0] = ZASTAVICA[1] = 0;
    *PRAVO = 0;

    pid_t pid = fork();
    if (pid == -1) {
        perror("fork failed");
        return 1;
    }

    if (pid == 0) {
        proc(0, ZASTAVICA, PRAVO);
        shmdt(ZASTAVICA);
        shmdt(PRAVO);
        exit(0);
    }
    else {
        proc(1, ZASTAVICA, PRAVO);
        wait(NULL);
        shmdt(ZASTAVICA);
        shmdt(PRAVO);
        shmctl(shmid_zastavica, IPC_RMID, NULL);
        shmctl(shmid_pravo, IPC_RMID, NULL);
    }

    return 0;
}



