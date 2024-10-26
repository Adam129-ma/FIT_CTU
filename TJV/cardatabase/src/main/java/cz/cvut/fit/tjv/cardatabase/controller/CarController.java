package cz.cvut.fit.tjv.cardatabase.controller;

import cz.cvut.fit.tjv.cardatabase.domain.Car;
import cz.cvut.fit.tjv.cardatabase.domain.Customer;
import cz.cvut.fit.tjv.cardatabase.service.CarService;
import cz.cvut.fit.tjv.cardatabase.service.CustomerService;
import cz.cvut.fit.tjv.cardatabase.service.DealerService;
import io.swagger.v3.oas.annotations.media.Content;
import io.swagger.v3.oas.annotations.media.Schema;
import io.swagger.v3.oas.annotations.responses.ApiResponse;
import io.swagger.v3.oas.annotations.responses.ApiResponses;
import org.springframework.http.HttpStatus;
import org.springframework.web.bind.annotation.*;
import org.springframework.web.server.ResponseStatusException;

import java.util.Collection;

@RestController
@CrossOrigin
@RequestMapping("/car")
public class CarController {

    private CarService carService;

    private CustomerService customerService;

    private DealerService dealerService;

    public CarController(CarService carService, CustomerService customerService, DealerService dealerService)
    {
        this.carService = carService;
        this.customerService = customerService;
        this.dealerService = dealerService;
    }

    @PostMapping
    @ResponseStatus(HttpStatus.OK)
    @ApiResponses({
            @ApiResponse(responseCode = "200", description = "Car created", content = { @Content(mediaType = "application/json",
                    schema = @Schema(implementation = Car.class)) })})
    public Car create (@RequestBody Car car ) {

        return carService.create(car);
    }


    @PostMapping("/{id}/customer")
    @ResponseStatus(HttpStatus.OK)
    @ApiResponses({
            @ApiResponse(responseCode = "409", description = "Car already has buyer with exact name!"),
            @ApiResponse(responseCode = "200", description = "Customer added",
                    content = { @Content(mediaType = "application/json",
                            schema = @Schema(implementation = Customer.class)) })
    })
    public Customer addCustomer (@PathVariable(value = "id") Long carId, @RequestBody Customer c) {
        Customer customer = customerService.create(c);

        try {
            carService.addCustomer(carId,customer.getId());
        }
        catch (IllegalArgumentException e)
        {
            customerService.deleteById(customer.getId());
            throw new ResponseStatusException(HttpStatus.CONFLICT);
        }

        return customer;
    }

    @DeleteMapping("/{id}/customer/{customerId}")
    @ResponseStatus(HttpStatus.OK)
    @ApiResponses({
            @ApiResponse(responseCode = "200", description = "Customer removed"),
            @ApiResponse(responseCode = "404", description = "invalid ID")
    })
    public void deleteCustomer (@PathVariable(value = "id") Long carId, @PathVariable(value = "customerId") Long customerId)
    {
        try {
            carService.deleteCustomer(carId,customerId);
        }
        catch (IllegalArgumentException e)
        {
            throw new ResponseStatusException(HttpStatus.NOT_FOUND);
        }
    }


    @ResponseStatus(HttpStatus.OK)
    @ApiResponses({
            @ApiResponse(responseCode = "200", description = ""),
            @ApiResponse(responseCode = "404", description = "invalid ID")
    })
    @GetMapping("{id}/notSoldBy")
    public Collection<String> getNotSoldBy (@PathVariable Long id)
    {
        if(carService.readById(id).isEmpty())
            throw new ResponseStatusException(HttpStatus.NOT_FOUND);

        return dealerService.getDealersNamesThatNotSoldCar(carService.readById(id).get());
    }

    @ResponseStatus(HttpStatus.OK)
    @ApiResponses(value = {
            @ApiResponse(responseCode = "200", description = "Cars found",
                    content = { @Content(mediaType = "application/json",
                            schema = @Schema(implementation = Car.class)) }),
    })
    @GetMapping
    public Iterable<Car> readAll() { return carService.readAll();}
    
    @ResponseStatus(HttpStatus.OK)
    @ApiResponses({
            @ApiResponse(responseCode = "200", description = "Car found"),
            @ApiResponse(responseCode = "404", description = "invalid ID")
    })
    @GetMapping("/{id}")
    public Car readOne(@PathVariable Long id) {
        if (carService.readById(id).isEmpty()) {
            throw new ResponseStatusException(HttpStatus.NOT_FOUND);
        } else {
            return carService.readById(id).get();
        }
    }


}
