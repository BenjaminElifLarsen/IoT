; ATMega 2560
.global RS232Init
.global Read

RS232Init:
  push r16
  ldi r16, 0 
  sts 0xC5, r16 ; UBRR0H
  ldi r16, 0x67
  sts 0xC4, 16 ; UBRR0L
  ldi r16, 0x80
  sts 0xC0, r16 ; UCSR0A
  ldi r16, (1 << 4) ; RXEN0
  sts 0xC1, r16 ; UCSR0B
  ldi r16, (1 << 2) | (1 << 1) ; UCSZ01 , UCSZ00 
  sts 0xC2, r16 ; UCSR0C
  pop r16
  ret 


Read:
  ;push r24
  lds r24, 0xC6 ; UDR0 
  ret
