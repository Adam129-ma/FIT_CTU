package cz.cvut.fit.tjv.cardatabase.service;

import cz.cvut.fit.tjv.cardatabase.domain.Car;

public interface CarService extends CrudService<Car, Long>{

    void addCustomer ( Long carId, Long customerId );

    void deleteCustomer ( Long carId, Long customerId );

}
