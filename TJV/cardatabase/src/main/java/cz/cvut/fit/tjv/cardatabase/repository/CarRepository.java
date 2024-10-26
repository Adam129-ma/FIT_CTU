package cz.cvut.fit.tjv.cardatabase.repository;

import cz.cvut.fit.tjv.cardatabase.domain.Car;
import org.springframework.data.repository.CrudRepository;
import org.springframework.stereotype.Repository;



@Repository
public interface CarRepository extends CrudRepository<Car, Long> {

}
