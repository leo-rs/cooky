// Jan Leo Ras
// CSCI 4100
// Programming Assignment 5
// This program is used to display the absolute path name of the current working directory.

#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <unistd.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>

void inum_to_filename(ino_t inum, char *pbuffer, int buff_sz);
int filename_to_inum(char * filename);
void display_path(ino_t inum);

//This function translates an inode number into the corresponding file name using the current working directory. 
//It takes three arguments: 
//the inode number to translate, a pointer to a buffer where the file name should be written, and the size of the buffer. 
//There is no return value.
void inum_to_filename(ino_t inum, char *pbuffer, int buff_sz)
{
  //open the current working directory
  //if there is a problem, display the appropriate error message and exit
  DIR *dir_ptr = opendir(".");
  if (&dir_ptr == NULL)
  {
    perror(".");
    exit(EXIT_FAILURE);
  }
  
  //read the first directory entry
  struct dirent *dirent_ptr = readdir(dir_ptr);
  
  //if the inode number in the curr dir entry matches the one passed to the function, 
  //copy the filename into the buffer and return from the function
  //Otherwise read the next directory entry and repeat the previous step.
  while (dirent_ptr != NULL)
  {
    if (filename_to_inum(dirent_ptr->d_name) == inum)
    {
      strncpy(pbuffer, dirent_ptr->d_name, buff_sz);
      pbuffer[buff_sz - 1] = '\0';
      return;
    }
    else
      dirent_ptr = readdir(dir_ptr);
  }
  
  //If the inode number passed to the function is not in any of the directory entries, 
  //display the appropriate error message and exit the program.
  fprintf(stderr, "error looking for inum %d\n", (int)inum);
  exit(EXIT_FAILURE);
  
  closedir(dir_ptr);
}

//This function does the opposite of the previous function: 
//it translates a filename to an inode number. It takes one argument, a char * representing the file name. 
//It returns the corresponding inode number, if one can be found.
int filename_to_inum(char *filename)
{
  //read the information from the file’s inode into a structure in memory.
  struct stat info;
  int result = stat(filename, &info);
  
  //If there is any problem getting this information, display the appropriate error message and exit the program.
  if (result == -1)
  {
    fprintf(stderr, "cannot stat ");
    perror(filename);
    exit(EXIT_FAILURE);
  }
  //Return the inode number from the structure in memory.
  return info.st_ino;
}

//This function is a recursive function that displays an absolute pathname for a given inode number. 
//It takes one argument, an inode number for the current working directory whose pathname it will display. 
//There is no return value.
void display_path(ino_t inum)
{
  //Create an array of characters to use as a buffer for the name of the directory.
  char pbuffer[BUFSIZ];

  //Get the inode number for the parent of the current directory using the filename_to_inum function.
  ino_t inum_parent = filename_to_inum("..");
  //If the parent inode number is the same as the current inode number, this means we have reached the root and can return from the function.
  if (inum_parent == inum) {
    return;
  }
  //Otherwise, change to the parent directory and use the inum_to_filename function to find the file name 
  //for the inode number that was passed to the function. 
  //Use the buffer declared in step 1 to store the file name.

  else
  {
  //Recursively call the display_path function to display the absolute path name of the parent directory.
  //Display a / followed by the file name in the buffer.
    chdir("..");
    inum_to_filename(inum, pbuffer, BUFSIZ);
    display_path(inum_parent);
    printf("/%s", pbuffer);
  }
}

int main(int argc, char *argv[])
{
  //Use the filename_to_inum function to get the inode number of the current working directory.
  //Pass the inode number to display_path to display the absolute path name of the current working directory.
  //Display a newline, so the command prompt appears on the next line.
  
  ino_t inum_cwd = filename_to_inum(".");
  printf("inum of cwd: %d", (int)inum_cwd);
  printf("\nfilename of inum (%d): \n", (int)inum_cwd);
  
  display_path(inum_cwd);
  printf("\n");
  
  return 0;
}
