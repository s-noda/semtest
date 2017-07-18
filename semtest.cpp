#include <iostream>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <chrono>
#include <thread>
// #include <errno.h>

int lock(int semid){
  struct timespec ts; ts.tv_sec=1; ts.tv_nsec=100*1000;
  struct sembuf sb; sb.sem_num=0; sb.sem_op=-1; sb.sem_flg=0;
  return semtimedop(semid,&sb,1,&ts); }

int unlock(int semid){
  // struct timespec ts; ts.tv_sec=0; ts.tv_nsec=1000*1000*1000;
  struct sembuf sb; sb.sem_num=0; sb.sem_op=+1; sb.sem_flg=0;
  // semtimedop(semid,&sb,1,&ts);
  return semop(semid,&sb,1); }

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
    if ( lock(semid) != 0 ) {
      std::cout << name << " : lock time outed" << std::endl;
    } else {
      std::cout << name << " start" << std::endl;
      std::cout << name << ": " << (_cnt++) << std::endl;
      std::chrono::milliseconds dura( 3000 );
      std::this_thread::sleep_for( dura );
      std::cout << name << " done" << std::endl;
      unlock(semid); }}

  return 100;
}
