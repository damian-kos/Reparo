# Reparo

Reparo is a local-based software which allows repair shops to take control over their repair orders.
Track you repairs, add, delete, edit them with ease.

## Installation

With first release `.exe` will be provided.

## Features

- **Platform:** Windows
- **User-Friendly:** Main motivation of this software was to make it quick to operate, inserting a new repair can be as little as 5 clicks
- **Devices database:** You can create your own databse of devices, with corresponding colors and aliases
- **Repairs status:** Easily sort through repair orders by status for a clearer overview of your workload. Add your own repair states.
- **Calendar:** Organize repairs based on dates. Sort through ID or customer number.
- **Notes:** Enables adding notes that can either be visible to the customer or hidden. 
- **Edit repair:** Modify repair details with a history log for reference.
- **Delete repairs:** Remove unwanted repairs orders from the system
- **Finances:**  Monitor your finances through the finances feature
- **Inventory system** Insert items to directory. Categorize them by Brand, Color, Quality and so on.
- **Barcodes** Print barcodes for your inventory and repairs.
- **Repair Ticket** You can print Repair Ticket, you can add your own logo and T&Cs to it.
- **Themes:** Choose between Dark and Light modes for your interface

### Adding a Customer
`New` > `Customer` > `Open Form`
When adding a new customer, the software will not allow you insert new customer if the customer already exists in the database. 
A red or green indicator bar will notify the user whether the customer details have been properly filled in. 

`Options` > `Fields options` : Here you can change what fields are necessary to fill for validation of customer.

![image](https://github.com/damian-kos/Reparo/assets/106775028/50f871a3-4cd6-4e38-bdcf-428c00e76f9f)
![image](https://github.com/damian-kos/Reparo/assets/106775028/983856ac-5541-4872-bc84-474a5eb93933)
![image](https://github.com/damian-kos/Reparo/assets/106775028/f94e1bd5-c1fb-4940-9cf6-cc4d83096ce4)

### Customers View
`View` > `Customers` > `Open Database`
In the Customers View, you have the ability to browse through all the customers recorded in your system. Customer are organized by their phone number.
You can seach through this databse by phone number. Table can be sorted by all columns.
![image](https://github.com/damian-kos/Reparo/assets/106775028/b94892f6-94d1-4332-b49d-a9118d3fb037)

- Customers related to any inserted `Repair` can't be removed
    ![image](https://github.com/damian-kos/Reparo/assets/106775028/16bccba4-d0cc-435c-8ce5-2f6184e3481a)
- Other Customers can be removed
    ![image](https://github.com/damian-kos/Reparo/assets/106775028/85335a96-f984-48cd-98ef-8385cb8ea515)

### Adding a Repair
When adding a new repair, you will see a window with Customer details and Device details. 
- **Customer section:** This section automatically searches the existing customer database using the phone number, which serves as a relatively unique identifier. 
Selecting a customer from the popup window automatically fills in the rest of their details. You can also insert new customer there.
- All repairs don't have default state. They are inserted in `All` label of state. You need to `Edit Repair` and set it to desired state.

![image](https://github.com/damian-kos/Reparo/assets/106775028/f343e58e-a8b1-4afd-98bf-ca1567bf1704)
![image](https://github.com/damian-kos/Reparo/assets/106775028/6d49f90d-a36e-40b6-ba6a-3bc02e4c621d)
![image](https://github.com/damian-kos/Reparo/assets/106775028/64be1533-757b-4535-9ea9-4b9b764f23c3)

`Options` > `Fields options` : Here you can change what fields are necessary to fill for validation of repair.

- **Device section:** This part allows you to search for devices using the comprehensive database, which includes device names sourced directly from official manufacturer data whenever possible.
  - As you type, a list of available devices will be suggested.
  
    ![image](https://github.com/damian-kos/Reparo/assets/106775028/82be491c-99bc-4f78-a04c-96ff64ac8f98)

  - Upon selecting a device, if the database contains color options specific to that model, these will be displayed for selection.
  
    ![image](https://github.com/damian-kos/Reparo/assets/106775028/20389266-dfcf-4317-85ab-15e0cdb0786d)

  -  `Custom Device`  If you will try to insert a repair with non-existent device it will create a custom device into separate database.
    
      ![image](https://github.com/damian-kos/Reparo/assets/106775028/a8e7b75d-6ca1-48bd-8998-2e9112753e87)


- **Category section:** You have the option to enter the determine to what `Category` this repair will be belonging to.
  - `Feature` You can add extra `Categories` to do so go to `Optons` > `Add attributes` > `Categories`.
- **SN / IMEI section:** You have the option to enter the serial number or IMEI of the device.
  - `Feature` This field can be left empty if you turn it off in `Options` > `Fields options`
- **Notes section:** You can add notes to each repair. One note is visible to the customer and will be printed on the ticket, while the other is private.
    - `Feature` This field can be left empty if you turn it off in `Options` > `Fields options`
- **Submit section:** Once all fields are completed and validated, the "Insert Repair" button will become active. Clicking this button will prompt a pop-up where you can confirm the addition of the repair or cancel the action.
  - `Feature` **Print Repair Ticket** You can print a Repair Ticket with details which were just entered. You can reprint Repair Ticket. o do so go to `View` > `Repairs` > `Open Database`. 
  In the window which just open, `Right Click` on a repair of which you want to print Repair Ticket.
  - `Feature` **Print Barcode** You can print a barcode with `Repair ID` and other data, like customer phone number, device name, and category. To do so go to `View` > `Repairs` > `Open Database`. 
  In the window which just open, `Right Click` on a repair of which you want to print Barcode.

### Edit Repair
`Right Click` on any `Repair` in `Repairs View`
The Edit Repair feature allows you to modify the details of an existing repair, including its status, price, or associated customer information. Here's how updates to customer details are handled:
- If you modify the details of the current customer and the new details do not match the phone number of any other customer in our database, new customer will be inserted into the database.
- If the new details entered match the phone number of another customer in our database, the repair will then be associated with this other customer.
- If the phone number will not be changed, current customer data will be modified.

![image](https://github.com/damian-kos/Reparo/assets/106775028/2501ea2d-e580-4fd3-a8f9-a3e2f3636afd)

All changes made to a repair order are documented in the `Updates` section. This ensures a clear history of modifications for each repair.

![image](https://github.com/damian-kos/Reparo/assets/106775028/7d5f0799-910f-4cb3-99a9-49405ec10ba6)


### Repairs View
`View` > `Repairs` > `Open Database`
In the Repairs View, you have the ability to browse through all the repairs recorded in your system. Repairs are organized by their current status, but you can also filter them basing on:
  - `Repair ID`
  - `Phone number`
  - `Before` a specified *date*
  - `After` a specified *date*
  - `Between` two specified *dates*
  - It is important to be aware that all filters will be applied at the same time.

![image](https://github.com/damian-kos/Reparo/assets/106775028/547aa97b-3557-45ba-b4d6-316d7d9cffc7)

- Table can be sorted on all given columns.
- You have the flexibility to hide certain columns or rearrange their order according to your preferences. `Right Click` on headers of table.

Right-clicking on a repair entry will bring up a popup menu that provides options to 
- `Update Repair` Opens a dialog of `Update repair details` where you can change all informations about repair and assign it to desired state.
- `Reprint Repair Ticket` Allows to reprint `Repair Ticket`
- `Print Barcode Label` Open a printing dialog to print a barcode with `Repair ID` and other data, like customer phone number, device name, and category.
- `Delete` Removes a repair from database. 

![image](https://github.com/damian-kos/Reparo/assets/106775028/a3350558-761d-430e-a3d4-09c99fe09ca0)

#### Insert New Device
`New` > `Device`
Allows to insert any device of your choice to databse of devices. You can set:
  ![image](https://github.com/damian-kos/Reparo/assets/106775028/1134e094-70da-446f-a036-e3e5598c9dc0)
- `Model` Defines a name of the device
  
  ![image](https://github.com/damian-kos/Reparo/assets/106775028/bf87d0cf-f527-4f0f-995a-b44c362bf9b4)

- `Type` Defines the type of device
  - `Feature` You can add extra `Types` to do so go to `Optons` > `Add attributes` > `Device Types`.
  
- `Brand` Defines a brand of the device
  - `Feature` You can add extra `Types` to do so go to `Optons` > `Add attributes` > `Brands`.




#### Upcoming Features
Stay tuned for these upcoming features to further enhance the functionality and user experience of Reparo.

- `Upcoming Feature` **Insert Customer** fields will be customizable, allowing users to decide which details are necessary, as some shops may opt not to collect comprehensive data from their customers.
- `Upcoming Feature` **Devices** Soon, you will be able to add a custom device, offering more flexibility to accommodate a wide range of repairs.
- `Upcoming Feature` **SN / IMEI** This field will be made editable, allowing you to require it as necessary for your record-keeping.
- `Upcoming Feature` **Notes section** This section will be made editable, enabling you to require notes as needed.
- `Upcoming Feature` **Print Repair Ticket** A feature to print the Repair Ticket, which can be handed to your customer for their reference.
- `Upcoming Feature` **Print Barcode** An option to print a Barcode for the repair, facilitating easier tracking and management of repair jobs.

