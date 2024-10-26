package cz.cvut.fit.tjv.cardatabase.service;

import cz.cvut.fit.tjv.cardatabase.domain.Car;
import cz.cvut.fit.tjv.cardatabase.domain.Customer;
import cz.cvut.fit.tjv.cardatabase.repository.CarRepository;
import cz.cvut.fit.tjv.cardatabase.repository.CustomerRepository;
import org.junit.jupiter.api.Assertions;
import org.junit.jupiter.api.Test;
import org.mockito.Mockito;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.boot.test.context.SpringBootTest;
import org.springframework.boot.test.mock.mockito.MockBean;

import java.util.ArrayList;
import java.util.Optional;

@SpringBootTest
public class CarServiceImplUnitTest {
    @Autowired
    private CarServiceImpl carService ;

    @MockBean
    private CarRepository carRepository;

    @MockBean
    private CustomerRepository customerRepository;

    @Test
    void addCustomers ( )
    {
        var car = new Car();
        var customer1 = new Customer();
        var customer2 = new Customer();

        car.setId(0L);
        car.setProductionYear(2009);
        car.setBrand("Tesla");
        car.setColour("Orange");
        car.setBoughtBy(new ArrayList<>());

        customer1.setId(7L);
        customer1.setName("Maria");
        customer2.setId(8L);
        customer2.setName("Julia");

        Mockito.when(carRepository.findById(car.getId())).thenReturn(Optional.of(car));
        Mockito.when(customerRepository.findById(customer1.getId())).thenReturn((Optional.of(customer1)));
        Mockito.when(customerRepository.findById(customer2.getId())).thenReturn((Optional.of(customer2)));

        carService.addCustomer(car.getId(),customer1.getId());
        carService.addCustomer(car.getId(),customer2.getId());

        Assertions.assertTrue(car.getBoughtBy().contains(customer1));
        Assertions.assertTrue(car.getBoughtBy().contains(customer2));
        Assertions.assertEquals(2, car.getBoughtBy().size());

        Mockito.verify(carRepository, Mockito.atLeast(2)).save(car);
    }

    @Test
    void addExistingCustomer ( )
    {
        var car = new Car();
        var customer1 = new Customer();
        var customer2 = new Customer();

        car.setId(0L);
        car.setProductionYear(2009);
        car.setBrand("Tesla");
        car.setColour("Orange");
        car.setBoughtBy(new ArrayList<>());

        customer1.setId(7L);
        customer1.setName("Maria");
        customer2.setId(8L);
        customer2.setName("Julia");

        Mockito.when(carRepository.findById(car.getId())).thenReturn(Optional.of(car));
        Mockito.when(customerRepository.findById(customer1.getId())).thenReturn((Optional.of(customer1)));
        Mockito.when(customerRepository.findById(customer2.getId())).thenReturn((Optional.of(customer2)));

        carService.addCustomer(car.getId(),customer1.getId());
        Assertions.assertThrows(IllegalArgumentException.class, () -> carService.addCustomer(car.getId(),customer1.getId()));
        Assertions.assertTrue(car.getBoughtBy().contains(customer1));
        Assertions.assertFalse(car.getBoughtBy().contains(customer2));
        Assertions.assertEquals(1, car.getBoughtBy().size());


        Mockito.verify(carRepository, Mockito.atLeast(1)).save(car);
        Mockito.verify(carRepository, Mockito.times(2)).findById(car.getId());
        Mockito.verifyNoMoreInteractions(carRepository);
    }

}
