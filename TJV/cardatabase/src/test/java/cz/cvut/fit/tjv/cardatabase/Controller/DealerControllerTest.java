package cz.cvut.fit.tjv.cardatabase.Controller;

import cz.cvut.fit.tjv.cardatabase.controller.DealerController;
import cz.cvut.fit.tjv.cardatabase.domain.Car;
import cz.cvut.fit.tjv.cardatabase.domain.Dealer;
import cz.cvut.fit.tjv.cardatabase.service.CarService;
import cz.cvut.fit.tjv.cardatabase.service.CustomerService;
import cz.cvut.fit.tjv.cardatabase.service.DealerService;
import org.hamcrest.Matchers;
import org.junit.jupiter.api.Test;
import org.junit.jupiter.api.extension.ExtendWith;
import org.mockito.Mockito;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.boot.test.autoconfigure.web.servlet.WebMvcTest;
import org.springframework.boot.test.mock.mockito.MockBean;
import org.springframework.http.MediaType;
import org.springframework.test.context.junit.jupiter.SpringExtension;
import org.springframework.test.web.servlet.MockMvc;
import org.springframework.test.web.servlet.request.MockMvcRequestBuilders;
import org.springframework.test.web.servlet.result.MockMvcResultMatchers;

@ExtendWith(SpringExtension.class)
@WebMvcTest(DealerController.class)
public class DealerControllerTest {
    @Autowired
    private MockMvc mockMvc;
    @MockBean
    private CarService carService;
    @MockBean
    private CustomerService customerService;
    @MockBean
    private DealerService dealerService;

    @Test
    void create() throws Exception {
        Dealer dealer = new Dealer();
        dealer.setPersonalNumber(25L);
        dealer.setName("Jozef");
        dealer.setAge(27);
        dealer.setNickname("Volvo");
        Mockito.when(dealerService.create(Mockito.any(Dealer.class))).thenReturn(dealer);

        mockMvc.perform(
                MockMvcRequestBuilders
                        .post("/dealer")
                        .contentType(MediaType.APPLICATION_JSON)
                        .content("{\"personalNumber\" : 25, \"name\" : \"Jozef\", \"age\": 27, \"nickname\": \"Volvo\" }")
        ).andExpect(
                MockMvcResultMatchers.status().isOk()
        ).andExpect(
                MockMvcResultMatchers.jsonPath("$.personalNumber", Matchers.is(25))
        );
    }
    @Test
    void deleteNonExistingDealer() throws Exception {
        Long nonExistingDealerId = 100L;

        Mockito.doThrow(new IllegalArgumentException("Invalid ID."))
                .when(dealerService).deleteById(nonExistingDealerId);

        mockMvc.perform(
                        MockMvcRequestBuilders
                                .delete("/dealer/" + nonExistingDealerId)
                                .accept(MediaType.APPLICATION_JSON)
                )
                .andExpect(
                        MockMvcResultMatchers.status().isNotFound()
                );
    }
}
