package cz.cvut.fit.tjv.cardatabase.service;

import cz.cvut.fit.tjv.cardatabase.domain.Car;
import cz.cvut.fit.tjv.cardatabase.domain.Customer;
import cz.cvut.fit.tjv.cardatabase.domain.Dealer;
import cz.cvut.fit.tjv.cardatabase.repository.CarRepository;
import cz.cvut.fit.tjv.cardatabase.repository.CustomerRepository;
import cz.cvut.fit.tjv.cardatabase.repository.DealerRepository;
import org.springframework.transaction.annotation.Transactional;
import org.junit.jupiter.api.Assertions;
import org.junit.jupiter.api.BeforeEach;
import org.junit.jupiter.api.Test;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.boot.test.context.SpringBootTest;

import java.util.ArrayList;

@SpringBootTest
public class CarIntegrationTest {
    @Autowired
    private DealerRepository dealerRepository;
    @Autowired
    private CarRepository carRepository;
    @Autowired
    private CustomerRepository customerRepository;
    @Autowired
    private CarServiceImpl carService;

    Dealer dealer = new Dealer();

    @BeforeEach
    void setUp()
    {
        dealerRepository.deleteAll();
        carRepository.deleteAll();
        customerRepository.deleteAll();
        dealer.setPersonalNumber(00L);

        dealer.setSoldCars(new ArrayList<>());

        dealerRepository.save(dealer);

        Car car = new Car();
        car.setId(02L);
        car.setProductionYear(2005);
        car.setColour("Red");
        car.setBrand("Skoda");
        car.setSoldBy(new ArrayList<>());
        car.setBoughtBy(new ArrayList<>());

        car.getSoldBy().add(dealer);
        dealer.getSoldCars().add(car);

        carRepository.save(car);
    }

    @Test
    @Transactional
    public void dealerSellCarToCustomer ( )
    {
        Customer customer = new Customer();

        Car carSoldByDealer = (Car) dealer.getSoldCars().toArray()[0];

        customer.setId(1L);
        customer.setName("Matej");
        customerRepository.save(customer);

        carSoldByDealer.setBoughtBy(new ArrayList<>());

        carService.addCustomer(carSoldByDealer.getId(),customer.getId());

        Assertions.assertFalse(carRepository.findById(carSoldByDealer.getId()).isEmpty());

        Assertions.assertTrue(carRepository.findById(carSoldByDealer.getId()).get().getBoughtBy().contains(customer));

        Assertions.assertEquals(1,carRepository.findById(carSoldByDealer.getId()).get().getBoughtBy().size());

    }

    @Test
    @Transactional
    public void editorDeleteNonExistingShapeFromHisCanvas ( )
    {
        Car carSoldByDealer = (Car) dealer.getSoldCars().toArray()[0];

        var customersThatBoughtCar = carSoldByDealer.getBoughtBy().size();

        Assertions.assertThrows(IllegalArgumentException.class, () -> carService.deleteCustomer(carSoldByDealer.getId(),76L));

        Assertions.assertEquals(customersThatBoughtCar, carSoldByDealer.getBoughtBy().size());
    }

}
