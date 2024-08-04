# Reparo

Reparo is a local-based software which allows repair shops to take control over their repair orders.
Track you repairs, add, delete, edit them with ease.

## Features

- **Platform:** Windows
- **User-Friendly:** Main motivation for this software was to make it quick and easy to operate. Inserting a new repair is as easy as 5 clicks
- **Devices database:** You can create your own database of devices, with corresponding colors and aliases
- **Repairs status:** Easily sort through repair orders by status for a clearer overview of your workload. Add your own repair states.
- **Calendar:** Organize repairs by dates. Sort through ID or customer number.
- **Notes:** Enables adding notes that can either be visible to the customer or hidden. 
- **Edit repair:** Modify repair details with a history log for reference.
- **Delete repairs:** Remove unwanted repairs orders from the system.
- **Finances:**  Monitor your finances through the finances feature.
- **Inventory system** Insert items to directory. Categorize them by Brand, Color, Quality and so on.
- **Barcodes** Print barcodes for your inventory and repairs.
- **Repair Ticket** You can print Repair Ticket, and add your own logo and T&Cs to it.
- **Themes:**Choose between Dark and Light modes for your interface.

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

### Manage Images
`Right Click` on any `Repair` in `Repairs View`
The Manage Images feature allows you to manage images for given `Repair`. At first there will be no images:
- To add images click `Upload Photos`:
  
   ![image](https://github.com/user-attachments/assets/c0684a43-ad5f-4708-8b61-fbfa08039b92)
  - File dialog will open. Maximum of 6 files at once can be chosen. `JPG` `jpg` `PNG` `png` files are accepted. It is important to choose correct filter.
  - For multiple selection either use `Ctrl` or `Shift`
    
    ![image](https://github.com/user-attachments/assets/55bfd2c5-65e3-4141-9d37-883ed2d6f23b)
- Loaded images will be displayed. From here you can `Left Click` on them to select them for deletion. Or use `Select All` and `Deselect All`. `Red` color means image is marked for deletion.

 ![image](https://github.com/user-attachments/assets/7af97f2c-6f3d-4513-9f79-d690424a0364)

- Hover over image to enlarge it.

  ![image](https://github.com/user-attachments/assets/9c9f6bea-744f-48ba-be4b-19cf9c238fe3)

- `Right Click` on an image to `Print` `Delete` or `Open File Location`.
  - Note: `Print` can only print one image at the time. To `Print` multiple use `Open File Location` and print from default file explorer.
  - Note: It is not advised to delete files from file explorer level.
    
    ![image](https://github.com/user-attachments/assets/74ad5007-3b74-42db-96c9-e88aa00e5c94)

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

### Insert New Device
`New` > `Device`
Allows to insert any device of your choice to database of devices. You can set:
    ![image](https://github.com/damian-kos/Reparo/assets/106775028/72ea3939-3f6d-4bf5-a325-290fbab4041e)

- `Model` Defines a name of the device

  ![image](https://github.com/damian-kos/Reparo/assets/106775028/a008ca12-1702-4b53-8df8-1b8e6ad41c2b)

- `Type` Defines the type of device
  - `Feature` You can add extra `Types` to do so go to `Optons` > `Add attributes` > `Device Types`.

    ![image](https://github.com/damian-kos/Reparo/assets/106775028/655b9c0a-5705-4254-864f-efadc2529722)

  
- `Brand` Defines a brand of the device
  - `Feature` You can add extra `Types` to do so go to `Optons` > `Add attributes` > `Brands`.

    ![image](https://github.com/damian-kos/Reparo/assets/106775028/8ddd6617-268e-4914-b627-c99595c3867b)

- `Colors` Defines a list of colors of the device

  ![image](https://github.com/damian-kos/Reparo/assets/106775028/c786b181-74b2-49cd-9929-78c94c499a2c)

- `Aliases` Defines a list of aliases of the device
  
  ![image](https://github.com/damian-kos/Reparo/assets/106775028/aa78c398-b871-4104-99d7-f2543ed7ef90)

- Example Device:
  
     ![image](https://github.com/damian-kos/Reparo/assets/106775028/48297e17-ebbb-432f-9a22-dad54a221eb4)

#### Insert New Device From Custom Devices
`New` > `New device from custom device` > `Open Database`
Allows to insert any device basing on a list of devices which were entered during inserting a repair, without adding the device itself to a list of devices.
If you insert devices with this information message:

![image](https://github.com/damian-kos/Reparo/assets/106775028/f7519a15-6116-4a5c-8fb5-f8c62285680b)

-  This device will then appear in `Custom Devices` table, like so:
  
    ![image](https://github.com/damian-kos/Reparo/assets/106775028/0308a42a-9e8b-43c3-a90a-024481b0c173)

- `Right Click` will open a Popup window, where you can this device to main database and change its details like we did in `New Device`

    ![image](https://github.com/damian-kos/Reparo/assets/106775028/3650385a-6f6f-4b1e-b80d-c57ae39fac85)

- `Delete` will remove this device from database of `Custom Devices` and it will be deleted from any associated repairs.
See example below what it did with repair number `2`. `Model` and `Color` are empty now.

  ![image](https://github.com/damian-kos/Reparo/assets/106775028/e80bfa5d-2b5a-4e32-b3e4-779e330bc562)

### Insert New Item
`New` > `Item` > `Open Form`
Allows to insert any item into the `Inventory`. Necessary fields to be filled out are `SKU` and `Item's name`.
Later you can print barcodes for these item with their `SKU` for better inventory tracking.

![image](https://github.com/damian-kos/Reparo/assets/106775028/fdac2198-f59f-45e9-98d4-715979976bdc)

- Fields explanation:
    - `Supplier` Suppliers name. After supplier is inserted once, it becomes avaiable to be picked up from a list of suppliers.
    - `Supplier's SKU` Supplier's SKU may be different from the one you desire to have for yours internal system. To make this happen make sure that, next field's checkbox is off.
    - `Shop's SKU same as Supplier's` Off to set different SKUs, On to copy `Supplier's SKU` into `SKU`
    - `Buy price (inc.VAT)` Buy price of an item including VAT.
    - `Ex. VAT buy price` If you want to calculate buy price ex. VAT check this box. Otherwise is gonna to be set to 0.
    - `VAT rate` Set your desired VAT rate basing on which ex. VAT buy price will be calculated.
    - `Quantity`
    - `Color` Color of an item from a list of available colors.
    - `Quality` Quality of an item.
        - `Feature` You can add extra `Qualities` to do so go to `Optons` > `Add attributes` > `Qualities`.
    - `Category` Category of an item.
        - `Feature` You can add extra `Categories` to do so go to `Optons` > `Add attributes` > `Categories`.
    - `Location` Item's location. After location is inserted once, it becomes avaiable to be picked up from a list of locatons.
    - `Box of compatible devices` Deterimine with what devices item is compatible with by choosing models from a list of devices at the bottom of this window.
To remove such device, just click on it in `red` button.
    - `For models` Set of filters which makes searching for compatible devices quicker.
  - After successful insertion you can print a barcode for inserted item.

     ![image](https://github.com/damian-kos/Reparo/assets/106775028/3819cd67-da73-4c3e-a7bf-c8960e7179af)
 
### View Inventory
`View` > `Inventory` > `Open Database`
Inserted items can be found in `Inventory`.  At the top of the window you will find filters which will help you search for given item.

![image](https://github.com/damian-kos/Reparo/assets/106775028/1e38f0f4-1935-4f27-a01a-5483caf08974)

From here you can:
- `Update Item` Change any details of an item. `SKU` can be only changed to other non-existing SKU in database.
- `Print Barcode Label` Print more barcodes.
- `Remove item` Removes item from a database.
- Adjust `Quantity` of an item. To do so, click on a number in `Quantity` column and edit the value in a popup.
  
  ![image](https://github.com/damian-kos/Reparo/assets/106775028/a845baf7-d759-4c71-bfb2-f8a79f986486)


### View Finances & Accounting
`View` > `Finances & Accounting` > `Open Database`
Allows to view summarized repairs prices sorted by `Repair States`. Can be filtered by date. Below you can find a table with repairs, from where you can do same actions as you would in `Repairs View`.
As for dates, it takes into consideration when repair was created.

![image](https://github.com/damian-kos/Reparo/assets/106775028/ff626c84-680b-4792-b06c-7c7aebfa51c0)

### View Devices
`View` > `Finances & Accounting` > `Open Database`
Views all `Devices` in databse, together with their `Colors` and `Aliases` from here by `Right Click` on any device you can edit these details.
If you remove a device, all associated repairs with this device will become blank. Like shown in `Insert New Device From Custom Devices`

![image](https://github.com/damian-kos/Reparo/assets/106775028/d0661d21-13a5-426f-8327-c4e2a8cc4beb)

## Options

### Theme 
`Options` > `Change theme` 
If you don't like current style of Reparo it can be set to other theme.

![Ro (1)](https://github.com/damian-kos/Reparo/assets/106775028/b3fc6a6c-5278-40d3-966b-ee96e191b7af)

### Load Company Logo
`Options` > `Load Company Logo` 
This options allows to choose a `.png` file as a logo which will appear on `Repair Ticket`. As default `Reparo` logo will appear. File Explorer will appear and will let you choose your file.

![image](https://github.com/damian-kos/Reparo/assets/106775028/d542de4b-c356-47d6-ae71-3371d6e5092e)

### Terms & Conditions
`Options` > `Terms & Conditions` 
This options allows set a `Terms & Conditions` which will  appear on `Repair Ticket`. As default there is no message set.

![image](https://github.com/damian-kos/Reparo/assets/106775028/aca0a41d-e0a0-4704-8ea0-3017a754f0d9)

### View Repair Ticket Template
`Options` > `View Repair Ticket Template` 
Views a current state of how `Repair Ticket` will be looking like with set `Logo` and `T&Cs`.

![image](https://github.com/damian-kos/Reparo/assets/106775028/9ab3e778-6112-4a2f-a366-b55c41da7267)

### Input fields options
`Options` > `Fields options` 
Allows to determine which fields can be empto for validation purposes. As a techinician myself I know that some people can't be bothered to collect necessary data. So not allowing to skip some fields, may force them to at least try to fill them.

![image](https://github.com/damian-kos/Reparo/assets/106775028/9a9128a8-e6e7-4409-b87e-f5691ba534a7)

### Add attributes
`Options` > `Add attribiutes` > `Brands` / `Categories` / `Qualities` / `Repair States` / `Device Types`
Througrough this application you can come across these attributes. This option allows you to add more of them or change them.
- `Brands` Adjust database of available brands to chose from. If brand associated with any `Item` or `Device` will be deleted, their `Brand` values will become either `Blank` or `Unknown`
- `Categories` Adjust database of available categories to chose from. If category associated with any `Repair` `Item` or `Device` will be deleted, their `Category` values will become `Blank`.
- `Qualities` Adjust database of available qualities to chose from. If quality associated with any `Item` their `Quality` values will become `Blank`
- `Repair States` Adjust database of available repair states to chose from. If repair state associated with any `Repair` will be deleted, state of this repair will become `All`.
- `Device Types` Adjust database of available device types to chose from. If devcice type associated with any `Device` will be deleted, their `Device Type` values will become either `Blank` or `Unknown`

  ![image](https://github.com/damian-kos/Reparo/assets/106775028/3f58f1cf-ed94-43ae-8f03-2a3e82e22e04)

### Check for updates
`Options` > `Check for updates`
This allow to download lates updates of an application. If there is no Update window will close. Otherwise a popup asking if you want to download an update will appear.
It is advised to do a copy of a database of your system. To do so go to the folder where you located `Reparo` and copy either whole `resource` directory, or `parts-stock.db` file from mentioned directory.

## Help & Feedback
Any feedback is more than welcome. To get help on how to use application either use a `Google Form` documents provided in these options or join the Discord Server.

## Credits
https://github.com/ocornut
https://github.com/aiekick
https://github.com/Flix01


#### Upcoming Features
Stay tuned for upcoming features to further enhance the functionality and user experience of Reparo.
- `Delete multiple` Delete multiple records
Any ideas, please let me know.
  
