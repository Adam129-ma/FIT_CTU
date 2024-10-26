package cz.cvut.fit.tjv.cardatabase.service;

import cz.cvut.fit.tjv.cardatabase.domain.Customer;
import cz.cvut.fit.tjv.cardatabase.repository.CustomerRepository;
import org.springframework.data.repository.CrudRepository;
import org.springframework.stereotype.Component;
import org.springframework.transaction.annotation.Transactional;

@Transactional
@Component
public class CustomerServiceImpl extends CrudServiceImpl<Customer,Long> implements CustomerService{
    private CustomerRepository customerRepository;

    public CustomerServiceImpl (CustomerRepository customerRepository) {
        this.customerRepository = customerRepository;
    }

    @Override
    protected CrudRepository<Customer, Long> getRepository(){return  customerRepository;}

}
