package cz.cvut.fit.tjv.cardatabase.service;

import cz.cvut.fit.tjv.cardatabase.domain.Car;
import cz.cvut.fit.tjv.cardatabase.domain.Customer;
import cz.cvut.fit.tjv.cardatabase.repository.CarRepository;
import cz.cvut.fit.tjv.cardatabase.repository.CustomerRepository;
import org.springframework.data.repository.CrudRepository;
import org.springframework.stereotype.Component;
import org.springframework.transaction.annotation.Transactional;

import java.util.Optional;
@Component
@Transactional
public class CarServiceImpl extends CrudServiceImpl<Car, Long> implements CarService{
    private CarRepository carRepository;

    private CustomerRepository customerRepository;

    public CarServiceImpl(CarRepository carRepository, CustomerRepository customerRepository)
    {
        this.carRepository = carRepository;
        this.customerRepository = customerRepository;
    }

    @Override
    public void addCustomer (Long carId, Long customerId)
    {
        Optional<Car> opCar = carRepository.findById(carId);
        Optional<Customer> opCustomer = customerRepository.findById(customerId);

        if ( opCar.isEmpty() || opCustomer.isEmpty())
            throw  new IllegalArgumentException("invalid ID");

        Car car = opCar.get();
        Customer customer = opCustomer.get();
        if ( car.getBoughtBy().stream().anyMatch(c -> c.getId().equals(customer.getId())))
            throw new IllegalArgumentException ( "Car is already being sold to this customer");


        customer.setBoughtCar(car);
        car.getBoughtBy().add(customer);

        carRepository.save(car);

    }

    @Override
    public void deleteCustomer (Long carId, Long customerId)
    {
        Optional<Car> opCar = carRepository.findById(carId);
        Optional<Customer> opCustomer = customerRepository.findById(customerId);

        if ( opCar.isEmpty() || opCustomer.isEmpty())
            throw  new IllegalArgumentException("invalid ID");

        Car car = opCar.get();
        Customer customer = opCustomer.get();

        if(!car.getBoughtBy().contains(customer))
            throw new IllegalArgumentException ( "Cannot delete non-existing customer!");

        car.getBoughtBy().remove(customer);
        customerRepository.deleteById(customerId);
    }

    @Override
    protected CrudRepository<Car,Long> getRepository() {return  carRepository;}

}
