package cz.cvut.fit.tjv.cardatabase.controller;

import cz.cvut.fit.tjv.cardatabase.domain.Car;
import cz.cvut.fit.tjv.cardatabase.domain.Dealer;
import cz.cvut.fit.tjv.cardatabase.service.CarService;
import cz.cvut.fit.tjv.cardatabase.service.DealerService;
import io.swagger.v3.oas.annotations.Operation;
import io.swagger.v3.oas.annotations.media.Content;
import io.swagger.v3.oas.annotations.media.Schema;
import io.swagger.v3.oas.annotations.responses.ApiResponse;
import io.swagger.v3.oas.annotations.responses.ApiResponses;
import org.springframework.http.HttpStatus;
import org.springframework.http.MediaType;
import org.springframework.web.bind.annotation.*;
import org.springframework.web.server.ResponseStatusException;

@RestController
@CrossOrigin
@RequestMapping("/dealer")
public class DealerController {

    private DealerService dealerService;

    private CarService carService;

    public DealerController(DealerService dealerService, CarService carService)
    {
        this.dealerService = dealerService;
        this.carService = carService;
    }
    @Operation
    @PostMapping
    @ResponseStatus(HttpStatus.OK)
    @ApiResponses({
            @ApiResponse(responseCode = "200", description = "Dealer created", content = { @Content(mediaType = "application/json",
                    schema = @Schema(implementation = Dealer.class)) }),
            @ApiResponse(responseCode = "400", description = "Dealer with same Id already exists"),
    })
    public Dealer create (@RequestBody Dealer dealer )
    {
        if(dealerService.readById(dealer.getId()).isPresent())
            throw new ResponseStatusException(HttpStatus.BAD_REQUEST);

        return dealerService.create(dealer);
    }

    @PatchMapping("/{id}")
    @ResponseStatus(HttpStatus.OK)
    @ApiResponses({
            @ApiResponse(responseCode = "200", description = "Dealer's name changed"),
            @ApiResponse(responseCode = "404", description = "invalid ID")
    })
    public void updateName (@PathVariable Long id, @RequestBody Dealer d ) {
        if(dealerService.readById(id).isEmpty())
            throw new ResponseStatusException(HttpStatus.NOT_FOUND);

        var editor = dealerService.readById(id).get();
        editor.setName(d.getName());

        dealerService.update(id,d);
    }

    @DeleteMapping("/{id}")
    @ResponseStatus(HttpStatus.OK)
    @ApiResponses({
            @ApiResponse(responseCode = "200", description = "Dealer removed"),
            @ApiResponse(responseCode = "404", description = "invalid ID")
    })
    public void deleteById(@PathVariable Long id) {
        try {
            dealerService.deleteById(id);
        } catch (IllegalArgumentException e) {
            throw new ResponseStatusException(HttpStatus.NOT_FOUND);
        }
    }

    @ResponseStatus(HttpStatus.OK)
    @ApiResponses(value = {
            @ApiResponse(responseCode = "200", description = "Dealers found",
                    content = { @Content(mediaType = "application/json",
                            schema = @Schema(implementation = Dealer.class)) }),
    })
    @GetMapping
    public Iterable<Dealer> readAll() { return dealerService.readAll();}


    @ResponseStatus(HttpStatus.OK)
    @ApiResponses(value = {
            @ApiResponse(responseCode = "404", description = "invalid ID"),
            @ApiResponse(responseCode = "200", description = "Editor found",
                    content = { @Content(mediaType = "application/json",
                            schema = @Schema(implementation = Dealer.class)) }),
    })
    @GetMapping("/{id}")
    public Dealer readOne(@PathVariable Long id) {
        if(dealerService.readById(id).isEmpty())
            throw new ResponseStatusException(HttpStatus.BAD_REQUEST);
        return dealerService.readById(id).get();
    }

    @PostMapping ("/{id}/soldCars")
    @ResponseStatus(HttpStatus.OK)
    @ApiResponses({
            @ApiResponse(responseCode = "200", description = "Car added to dealer's collection", content = { @Content(mediaType = "application/json",
                    schema = @Schema(implementation = Car.class)) }),
            @ApiResponse(responseCode = "404", description = "invalid ID"),
            @ApiResponse(responseCode = "409", description = "Dealer already sold this car")
    })
    public void addCar (@PathVariable Long id, @RequestBody Car c) {

        if(carService.readById(c.getId()).isEmpty() || dealerService.readById(id).isEmpty())
            throw new ResponseStatusException(HttpStatus.NOT_FOUND);

        try
        {
            dealerService.addCar(id,c.getId());
        } catch (IllegalArgumentException e)
        {
            throw new ResponseStatusException(HttpStatus.CONFLICT);
        }
    }

    @DeleteMapping ("/{id}/soldCars/{carId}")
    @ResponseStatus(HttpStatus.OK)
    @ApiResponses({
            @ApiResponse(responseCode = "200", description = "Car removed from dealer's collection"),
            @ApiResponse(responseCode = "404", description = "invalid ID")
    })
    public void removeCar (@PathVariable Long id, @PathVariable Long carId) {
        var vectorCanvas = carService.readById(carId);

        if ( vectorCanvas.isEmpty() || dealerService.readById(id).isEmpty() )
            throw new ResponseStatusException(HttpStatus.BAD_REQUEST);

        dealerService.removeCar(id,carId);
    }

}
