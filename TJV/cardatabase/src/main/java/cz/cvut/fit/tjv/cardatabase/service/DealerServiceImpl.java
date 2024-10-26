package cz.cvut.fit.tjv.cardatabase.service;

import cz.cvut.fit.tjv.cardatabase.domain.Car;
import cz.cvut.fit.tjv.cardatabase.domain.Dealer;
import cz.cvut.fit.tjv.cardatabase.repository.CarRepository;
import cz.cvut.fit.tjv.cardatabase.repository.CustomerRepository;
import cz.cvut.fit.tjv.cardatabase.repository.DealerRepository;
import org.springframework.data.repository.CrudRepository;
import org.springframework.stereotype.Component;
import org.springframework.transaction.annotation.Transactional;

import java.util.Collection;
@Component
@Transactional
public class DealerServiceImpl extends CrudServiceImpl<Dealer, Long> implements DealerService{
    private DealerRepository dealerRepository;

    private CarRepository carRepository;

    private CustomerRepository customerRepository;

    public DealerServiceImpl ( DealerRepository dealerRepository, CarRepository carRepository, CustomerRepository customerRepository) {
        this.dealerRepository = dealerRepository;
        this.carRepository = carRepository;
        this.customerRepository = customerRepository;
    }

    @Override
    public void addCar(Long dealerId, Long carId) {

        var Car = carRepository.findById(carId).get();
        var dealer = dealerRepository.findById(dealerId).get();

        if(dealer.getSoldCars().contains(Car))
            throw new IllegalArgumentException();

        dealer.getSoldCars().add(Car);
        update(dealerId,dealer);
    }

    @Override
    public void removeCar (Long dealerId, Long carId)
    {
        var Car = carRepository.findById(carId).get();
        var currentDealer = dealerRepository.findById(dealerId).get();
        currentDealer.getSoldCars().remove(carId);
        update(dealerId,currentDealer);

        var editing = getDealersNamesThatSoldCar(Car);

        if(!editing.isEmpty())
            return;

        for ( var shape : Car.getBoughtBy())
            customerRepository.deleteById(shape.getId());

        carRepository.deleteById(carId);
    }

    @Override
    public Collection<String> getDealersNamesThatSoldCar(Car car)
    {
        return dealerRepository.getDealersNamesThatSoldCar(car);
    }

    @Override
    public Collection<String> getDealersNamesThatNotSoldCar(Car car)
    {
        return dealerRepository.getDealersNamesThatNotSoldCar(car);
    }

    @Override
    protected CrudRepository<Dealer, Long> getRepository(){return  dealerRepository;}

}
