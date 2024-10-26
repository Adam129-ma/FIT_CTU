package cz.cvut.fit.tjv.cardatabase.repository;

import cz.cvut.fit.tjv.cardatabase.domain.Car;
import cz.cvut.fit.tjv.cardatabase.domain.Dealer;
import org.springframework.data.jpa.repository.Query;
import org.springframework.data.repository.CrudRepository;
import org.springframework.stereotype.Repository;

import java.util.Collection;

@Repository
public interface DealerRepository extends CrudRepository<Dealer, Long> {

    @Query("SELECT d.name FROM Dealer d WHERE :car member of d.soldCars")
    Collection<String> getDealersNamesThatSoldCar(Car car);
    @Query("SELECT name From Dealer WHERE name NOT IN(SELECT d.name FROM Dealer d WHERE :car member of d.soldCars)")
    Collection<String> getDealersNamesThatNotSoldCar(Car car);
}
