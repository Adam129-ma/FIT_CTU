package cz.cvut.fit.tjv.cardatabase.controller;

import cz.cvut.fit.tjv.cardatabase.domain.Car;
import cz.cvut.fit.tjv.cardatabase.domain.Customer;
import cz.cvut.fit.tjv.cardatabase.service.CustomerService;
import io.swagger.v3.oas.annotations.Operation;
import io.swagger.v3.oas.annotations.media.Content;
import io.swagger.v3.oas.annotations.media.Schema;
import io.swagger.v3.oas.annotations.responses.ApiResponse;
import io.swagger.v3.oas.annotations.responses.ApiResponses;
import org.springframework.http.HttpStatus;
import org.springframework.web.bind.annotation.ResponseStatus;
import org.springframework.web.bind.annotation.RestController;
import org.springframework.web.bind.annotation.*;
import org.springframework.web.server.ResponseStatusException;


@RestController
@CrossOrigin
@RequestMapping("/customer")
public class CustomerController {
    private CustomerService customerService;
    public CustomerController(CustomerService customerService){
        this.customerService = customerService;
    }
   @ResponseStatus(HttpStatus.OK)
   @ApiResponses(value = {
           @ApiResponse(responseCode = "200", description = "Customers found",
           content = {@Content(mediaType = "application/json", schema = @Schema(implementation = Customer.class))})})
   @GetMapping
    public Iterable<Customer> readAll() {
        return customerService.readAll();
    }
    @GetMapping("/{id}")
    public Customer readOne(@PathVariable Long id) {
        if (customerService.readById(id).isEmpty()) {
            throw new ResponseStatusException(HttpStatus.NOT_FOUND);
        } else {
            return customerService.readById(id).get();
        }
    }
}
