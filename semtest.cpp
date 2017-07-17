#include <iostream>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <chrono>
#include <thread>

void lock(int semid){
  struct sembuf sb; sb.sem_num=0; sb.sem_op=-1; sb.sem_flg=0;
  if(semop(semid,&sb,1) == -1){
    std::cout << "lock failed" << std::endl; exit(1); }}

void unlock(int semid){
  struct sembuf sb; sb.sem_num=0; sb.sem_op=+1; sb.sem_flg=0;
  if(semop(semid,&sb,1) == -1){
    std::cout << "unlock failed" << std::endl; exit(1); }}

int main (int argc, char** argv) {
  int semid;
  int _cnt=0;
  const char* name = ((argc > 1) ? argv[1] : "parent");
  std::cout << name << " starting" << std::endl;
  if ((semid = semget(4444, 1, 0666 | IPC_CREAT)) == -1){
    std::cout << "semget failed" << std::endl;
    return -1; }
  if ( argc == 1 ) unlock(semid);

  while ( true ) {
    lock(semid);
    std::cout << name << ": " << (_cnt++) << std::endl;
    std::cout << name << " start" << std::endl;
    std::chrono::milliseconds dura( 3000 );
    std::this_thread::sleep_for( dura );
    std::cout << name << " done" << std::endl;
    unlock(semid); }

  return 100;
}
