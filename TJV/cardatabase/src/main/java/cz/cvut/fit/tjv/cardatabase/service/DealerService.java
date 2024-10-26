package cz.cvut.fit.tjv.cardatabase.service;

import cz.cvut.fit.tjv.cardatabase.domain.Car;
import cz.cvut.fit.tjv.cardatabase.domain.Dealer;

import java.util.Collection;

public interface DealerService extends CrudService<Dealer, Long>{
    void addCar (Long dealerId, Long carId);

    void removeCar (Long dealerId, Long carId);

    Collection<String> getDealersNamesThatSoldCar(Car car);

    Collection<String> getDealersNamesThatNotSoldCar(Car car);
}
