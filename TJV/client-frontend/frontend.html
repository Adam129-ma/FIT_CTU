<!DOCTYPE html>
<html>
<head>
    <title>Customer Frontend</title>
    <script>
        async function fetchAllCustomers() {
            try {
                const response = await fetch('http://localhost:8090/customer', {
                    method: 'GET'
                });
                const data = await response.json();
                console.log('All Customers:', data);
            } catch (error) {
                console.error('Error fetching all customers:', error);
            }
        }

        async function fetchCustomerById() {
            const customerId = document.getElementById('customerId').value;
            if (customerId.trim() !== '') {
                try {
                    const response = await fetch(`http://localhost:8090/customer/${customerId}`, {
                        method: 'GET'
                    });
                    if (!response.ok) {
                        throw new Error('Customer not found');
                    }
                    const data = await response.json();
                    console.log('Customer by ID:', data);
                } catch (error) {
                    console.error('Error fetching customer by ID:', error);
                }
            } else {
                console.error('Please enter a valid customer ID');
            }
        }
    </script>
</head>
<body>
    <h1>Customers</h1>
    <button onclick="fetchAllCustomers()">Get All Customers</button>
    <br><br>
    <label for="customerId">Enter Customer ID:</label>
    <input type="number" id="customerId" name="customerId">
    <button onclick="fetchCustomerById()">Get Customer by ID</button>
</body>
 <title>Car Frontend</title>
    <script>
        async function createCar() {
            const carData = {
                // Add necessary car data here (e.g., make, model, year, etc.)
            };

            try {
                const response = await fetch('http://localhost:8090/car', {
                    method: 'POST',
                    headers: {
                        'Content-Type': 'application/json'
                    },
                    body: JSON.stringify(carData)
                });
                const createdCar = await response.json();
                console.log('Car created:', createdCar);
            } catch (error) {
                console.error('Error creating car:', error);
            }
        }

        async function addCustomerToCar() {
            const carId = document.getElementById('carId').value;
            const customerData = {
                // Add necessary customer data here (e.g., name, email, etc.)
            };

            if (carId.trim() !== '') {
                try {
                    const response = await fetch(`http://localhost:8090/car/${carId}/customer`, {
                        method: 'POST',
                        headers: {
                            'Content-Type': 'application/json'
                        },
                        body: JSON.stringify(customerData)
                    });
                    const addedCustomer = await response.json();
                    console.log('Customer added to car:', addedCustomer);
                } catch (error) {
                    console.error('Error adding customer to car:', error);
                }
            } else {
                console.error('Please enter a valid car ID');
            }
        }

        async function deleteCustomerFromCar() {
            const carId = document.getElementById('carId').value;
            const customerId = document.getElementById('customerId').value;

            if (carId.trim() !== '' && customerId.trim() !== '') {
                try {
                    await fetch(`http://localhost:8090/car/${carId}/customer/${customerId}`, {
                        method: 'DELETE'
                    });
                    console.log('Customer deleted from car');
                } catch (error) {
                    console.error('Error deleting customer from car:', error);
                }
            } else {
                console.error('Please enter valid car and customer IDs');
            }
        }

        async function getNotSoldBy() {
            const carId = document.getElementById('carId').value;

            if (carId.trim() !== '') {
                try {
                    const response = await fetch(`http://localhost:8090/car/${carId}/notSoldBy`, {
                        method: 'GET'
                    });
                    const notSoldByList = await response.json();
                    console.log('Dealers not selling this car:', notSoldByList);
                } catch (error) {
                    console.error('Error getting dealers not selling this car:', error);
                }
            } else {
                console.error('Please enter a valid car ID');
            }
        }

        async function fetchAllCars() {
            try {
                const response = await fetch('http://localhost:8090/car', {
                    method: 'GET'
                });
                const allCars = await response.json();
                console.log('All Cars:', allCars);
            } catch (error) {
                console.error('Error fetching all cars:', error);
            }
        }

        async function fetchCarById() {
            const carId = document.getElementById('carId').value;
            if (carId.trim() !== '') {
                try {
                    const response = await fetch(`http://localhost:8090/car/${carId}`, {
                        method: 'GET'
                    });
                    const carById = await response.json();
                    console.log('Car by ID:', carById);
                } catch (error) {
                    console.error('Error fetching car by ID:', error);
                }
            } else {
                console.error('Please enter a valid car ID');
            }
        }
    </script>
</head>
<body>
    <h1>Cars</h1>
    <button onclick="createCar()">Create Car</button>
    <br><br>
    <label for="carId">Car ID:</label>
    <input type="number" id="carId" name="carId">
    <button onclick="fetchCarById()">Get Car by ID</button>
    <br><br>
    <button onclick="fetchAllCars()">Get All Cars</button>
    <br><br>
    <button onclick="addCustomerToCar()">Add Customer to Car</button>
    <br><br>
    <label for="customerId">Customer ID:</label>
    <input type="number" id="customerId" name="customerId">
    <button onclick="deleteCustomerFromCar()">Delete Customer from Car</button>
    <br><br>
    <button onclick="getNotSoldBy()">Get Dealers Not Selling This Car</button>
</body>
<head>
    <title>Dealers</title>
    <script>
        
        async function createDealer() {
            const personalNumber = document.getElementById('personalNumber').value;
            const age = document.getElementById('age').value;
            const name = document.getElementById('name').value;
            const nickname = document.getElementById('nickname').value;

            const dealerData = {
                personalNumber: parseInt(personalNumber),
                age: parseInt(age),
                name: name,
                nickname: nickname || null,
                soldCars: [] // Initialize soldCars as an empty array
            };

            try {
                // Fetch all dealers
                const allDealersResponse = await fetch('http://localhost:8090/dealer', {
                    method: 'GET'
                });
                const allDealers = await allDealersResponse.json();

                // Check if dealer with the same personal number already exists
                const existingDealer = allDealers.find(dealer => dealer.personalNumber === dealerData.personalNumber);
                if (existingDealer) {
                    console.log('Dealer with this Personal Number already exists:', existingDealer);
                    return; // Exit function if dealer with the same personal number already exists
                }

                // Create the dealer if no existing dealer found with the same personal number
                const createDealerResponse = await fetch('http://localhost:8090/dealer', {
                    method: 'POST',
                    headers: {
                        'Content-Type': 'application/json'
                    },
                    body: JSON.stringify(dealerData)
                });
                const createdDealer = await createDealerResponse.json();
                console.log('Dealer created:', createdDealer);
            } catch (error) {
                console.error('Error creating dealer:', error);
            }
        }

        async function updateDealerName() {
            const dealerId = document.getElementById('dealerId').value;
            const updatedName = document.getElementById('updatedName').value;

            const updatedDealerData = {
                name: updatedName
            };

            if (dealerId.trim() !== '') {
                try {
                    const response = await fetch(`http://localhost:8090/dealer/${dealerId}`, {
                        method: 'PATCH',
                        headers: {
                            'Content-Type': 'application/json'
                        },
                        body: JSON.stringify(updatedDealerData)
                    });
                    console.log('Dealer name updated');
                } catch (error) {
                    console.error('Error updating dealer name:', error);
                }
            } else {
                console.error('Please enter a valid dealer ID');
            }
        }

        async function deleteDealerById() {
            const deleteDealerId = document.getElementById('deleteDealerId').value;

            if (deleteDealerId.trim() !== '') {
                try {
                    await fetch(`http://localhost:8090/dealer/${deleteDealerId}`, {
                        method: 'DELETE'
                    });
                    console.log('Dealer deleted');
                } catch (error) {
                    console.error('Error deleting dealer:', error);
                }
            } else {
                console.error('Please enter a valid dealer ID');
            }
        }

        async function fetchAllDealers() {
            try {
                const response = await fetch('http://localhost:8090/dealer', {
                    method: 'GET'
                });
                const allDealers = await response.json();
                console.log('All Dealers:', allDealers);
            } catch (error) {
                console.error('Error fetching all dealers:', error);
            }
        }

        async function fetchDealerById() {
            const dealerId = document.getElementById('dealerId').value;
            if (dealerId.trim() !== '') {
                try {
                    const response = await fetch(`http://localhost:8090/dealer/${dealerId}`, {
                        method: 'GET'
                    });
                    const dealerById = await response.json();
                    console.log('Dealer by ID:', dealerById);
                } catch (error) {
                    console.error('Error fetching dealer by ID:', error);
                }
            } else {
                console.error('Please enter a valid dealer ID');
            }
        }

        async function addCarToDealer() {
            const dealerId = document.getElementById('dealerId').value;
            const productionYear = document.getElementById('productionYear').value;
            const colour = document.getElementById('colour').value;
            const brand = document.getElementById('brand').value;

            const carData = {
                productionYear: parseInt(productionYear),
                colour: colour,
                brand: brand
            };

            if (dealerId.trim() !== '') {
                try {
                    const response = await fetch(`http://localhost:8090/dealer/${dealerId}/soldCars`, {
                        method: 'POST',
                        headers: {
                            'Content-Type': 'application/json'
                        },
                        body: JSON.stringify(carData)
                    });
                    console.log('Car added to dealer');
                } catch (error) {
                    console.error('Error adding car to dealer:', error);
                }
            } else {
                console.error('Please enter a valid dealer ID');
            }
        }

        async function removeCarFromDealer() {
            const dealerId = document.getElementById('deleteCarDealerId').value;
            const carId = document.getElementById('deleteCarId').value;

            if (dealerId.trim() !== '' && carId.trim() !== '') {
                try {
                    await fetch(`http://localhost:8090/dealer/${dealerId}/soldCars/${carId}`, {
                        method: 'DELETE'
                    });
                    console.log('Car removed from dealer');
                } catch (error) {
                    console.error('Error removing car from dealer:', error);
                }
            } else {
                console.error('Please enter valid dealer and car IDs');
            }
        }
    </script>
</head>
<body>
    <h1>Dealers</h1>
    <label for="personalNumber">Personal Number:</label>
    <input type="number" id="personalNumber" name="personalNumber">
    <br><br>
    <label for="age">Age:</label>
    <input type="number" id="age" name="age">
    <br><br>
    <label for="name">Name:</label>
    <input type="text" id="name" name="name">
    <br><br>
    <label for="nickname">Nickname:</label>
    <input type="text" id="nickname" name="nickname">
    <br><br>

    <button onclick="createDealer()">Create Dealer</button>
    <label for="dealerId">Dealer ID:</label>
    <input type="number" id="dealerId" name="dealerId">
    <button onclick="fetchDealerById()">Get Dealer by ID</button>
    <br><br>

    <button onclick="fetchAllDealers()">Get All Dealers</button>
    <br><br>

    <label for="dealerId">Dealer ID:</label>
    <label for="updatedName">Updated Name:</label>
    <input type="text" id="updatedName" name="updatedName">
    <button onclick="updateDealerName()">Update Dealer Name</button>
    <br><br>

    <label for="productionYear">Production Year:</label>
    <input type="number" id="productionYear" name="productionYear">
    <br><br>

    <label for="colour">Colour:</label>
    <input type="text" id="colour" name="colour">
    <br><br>

    <label for="brand">Brand:</label>
    <input type="text" id="brand" name="brand">
    <br><br>

    <button onclick="addCarToDealer()">Add Car to Dealer</button>
    <br><br>

    <label for="deleteCarDealerId">Dealer ID:</label>
    <input type="number" id="deleteCarDealerId" name="deleteCarDealerId">
    <br><br>

    <label for="deleteCarId">Car ID:</label>
    <input type="number" id="deleteCarId" name="deleteCarId">
    <button onclick="removeCarFromDealer()">Remove Car from Dealer</button>
</body>
</html>
