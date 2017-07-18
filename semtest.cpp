#include <iostream>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <chrono>
#include <thread>
#include <assert.h>

bool lock(int semid){
  struct timespec ts1s = {0, 100*1000};
  struct sembuf sblock = {0, -1, 0};
  return (semtimedop(semid,&sblock,1,&ts1s) == 0); }

bool unlock(int semid) {
  struct sembuf sbunlock = {0, +1, 0};
  return (semtimedop(semid,&sbunlock,1, NULL) == 0); }

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
    if ( ! lock(semid) ) {
      // std::cout << name << " : lock time outed" << std::endl;
    } else {
      std::cout << name << " start(" << (++_cnt) << ")" << std::endl;
      std::chrono::milliseconds dura( 3000 );
      std::this_thread::sleep_for( dura );
      std::cout << name << " done (" << _cnt << ")" << std::endl;
      assert(unlock(semid)); }}
  return 100;
}
