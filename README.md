# OS-Project3

Members: Aldin Talić, Admir Nedžibović

Listing of files/directories in submission: Main folder "OS-Project3" contains: README.md, program.c (code of the program). 

******************

- In the code, synchronisation is achieved using semaphores and a mutex. The mutex ensures that only one thread can can access the shared resource or critical section which is 'waiting_count'. The customers semaphore is used to track the number of waiting customers and the barber semaphore signals the arrival of a customer.
- The customer_thread function represents the entry point for customer threads. It is checked if there is an available chair. If so, the barber is told that there is a new customer and the customer waits for the barber to be available. If there are no free chairs, the customer leaves.
- The barber_thread function represents the entry point for the barber thread. The barber first sleeps, waiting for a customer. A customer is notified that the barber is available and the waiting_count is decremented since the barber will now cut a customer. 