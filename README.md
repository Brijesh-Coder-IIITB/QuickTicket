# Quick Ticket
### An Interactive CLI based Railway Reservation &amp; Management System. 

#### Downloading and Executing the Project
Download the project from GitHub or clone it using the following command.

```
git clone https://github.com/tatwik-sai/QuickTicket.git
```

- Navigate into the QuickTicket directory before running the program.
- Ensure to have the make utility installed on your system.

```
sudo apt install make
```

- Run the following command to create an executable (.out) file.

```
make rerun
```

- Make sure the terminal is in full screen mode to avoid any display issues.
- Now, run the executable file using the following command.

```
./QuickTicket.out
```

>**Note:** This program works only on Linux based systems.  


---

![Home Page](https://i.ibb.co/8rSTXVk/qkt1.png)
---

>**Note:** To access the admin panel use one of the credentials below.  

| Username   | Password       |
|------------|----------------|
| authority  | somepass       |
| tatwik_sai | verystrongpass |

---

#### Team Members

| Name   | Rollno       | Contribution  |
|------------|----------------|---------|
|Madire Shiva Reddy | BT2024218 | Handling the database |
|Molleti Tatwik Sai | BT2024200 | Ticket Booking, Interface, Some Utility functions |
| Mohammed Farhan | BT2024140 | Removing a Schedule and viewing reservations from admin side |
| Mallepaddi Bhavesh | IMT2024069 | Management of Trains from the Admin side.|
| Chandala Tharun Sai Pranav | BT2024168 | Viewing and searching Trains and login functionality. |
| Repalle Dhanush | BT2024155 | Handling reservations and updating profile from user side. |

---

#### Application Features
- The application starts with a login page where you can register as a new user or sign in as an admin or a user .
    - **Admin** - You can sign in using the default credentials.
    - **User** - Can sign in or sign up.

- Once you sign in, you will be taken to the main page with different options for admin and user.
    - **Admin**
      
        **1.** View Trains  
        **2.** View Schedules  
        **3.** View Schedules of a Train  
        **4.** Add Train  
        **5.** Remove Train  
        **6.** Add Schedule  
        **7.** Remove Schedule  
        **8.** View Reservations  
        **9.** View Reservations of a Train Schedule  
        **0.** Exit

    - **User**
      
        **1.** View all Trains  
        **2.** Search Trains from Source to Destination  
        **3.** Search Trains from Source to Destination on a Date  
        **4.** Book a Ticket    
        **5.** View Train Map  
        **6.** View Reservation History   
        **7.** View Active Reservations  
        **8.** Cancel a Reservation  
        **9.** Update Profile  
        **0.** Exit  
---
