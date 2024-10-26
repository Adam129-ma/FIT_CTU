package cz.cvut.fit.tjv.cardatabase.repository;

import cz.cvut.fit.tjv.cardatabase.domain.Customer;
import org.springframework.data.repository.CrudRepository;
import org.springframework.stereotype.Repository;

@Repository
public interface CustomerRepository extends CrudRepository<Customer, Long> {
}
