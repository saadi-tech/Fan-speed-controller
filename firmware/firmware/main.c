/*
 * firmware.c
 *
 * Created: 11/5/2020 1:36:43 PM
 * Author : GAndoo
 */ 

#define  F_CPU 8000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>


void init_pwm(uint8_t );

void set_PWM(uint8_t);

void uart_init(int baud_rate);
void serial_write(unsigned char data[]);
void serial_writeln(unsigned char data[]);

volatile uint16_t current_speed = 0;
uint16_t adc_val = 0;

char string[20];

int main(void)
{
    /* Replace with your application code */
	init_pwm(0);
	
	uart_init(9600);
	sei();
    while (1) 
	    {
			set_PWM(current_speed);
    }
}




void init_pwm(uint8_t value){
	DDRD |= (1 << DDD6);
	// PD6 is now an output

	OCR0A = (value * 255)/100;
	// set PWM for 50% duty cycle


	TCCR0A |= (1 << COM0A1);
	// set none-inverting mode

	TCCR0A |= (1 << WGM01) | (1 << WGM00);
	// set fast PWM Mode

	TCCR0B |= (1 << CS01);
	// set prescaler to 8 and starts PWM


}

void set_PWM(uint8_t value){
	OCR0A = (value * 255)/100;
	// set PWM for value% duty cycle
	
	
}






void uart_init(int baud_rate){
	
	//int value =     ((freq)/(16*baud_rate)) - 1;
	int value = 51;
	UBRR0H = (value>>8);
	UBRR0L = (value);
	
	UCSR0C = 0x06;       /* Set frame format: 8data, 1stop bit  */
	
	UCSR0B = (1<<TXEN0) | (1<<RXCIE0) | (1<<RXEN0); /* Enable  transmitter                 */
}

void serial_write(unsigned char data[]){
	int length = strlen(data);
	int i = 0;
	for (i = 0; i < length ; i++){
		UDR0 = data[i];
		while (!( UCSR0A & (1<<UDRE0))); /* Wait for empty transmit buffer*/
		
		
	}
	
}

void serial_writeln(unsigned char data[]){
	int length = strlen(data);
	int i = 0;
	for (i = 0; i < length ; i++){
		UDR0 = data[i];
		while (!( UCSR0A & (1<<UDRE0))); /* Wait for empty transmit buffer*/
		
		
	}
	
	serial_write("\n\r");
	
}

ISR (USART_RX_vect){
	
	char ReceivedChar = UDR0;
	
	if (ReceivedChar != '\n' && ReceivedChar != '\r'){
	if (ReceivedChar == 's'){
		current_speed = 0;
		serial_write("Speed set: ");
		itoa(current_speed,string,10);
		serial_writeln(string);
	}
	else{
	current_speed = 10 + 10*(ReceivedChar-'0');
	serial_write("Speed set: ");
	itoa(current_speed,string,10);
	serial_writeln(string);
}
	}
}