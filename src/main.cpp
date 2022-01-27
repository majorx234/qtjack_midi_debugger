#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdint.h>
#include <assert.h>

#include <jack/jack.h>
#include <jack/midiport.h>

int64_t rcf_midi_msgs_count;
typedef union convert_8BytesToInt64{
  int64_t integer;
  unsigned char byte[8];
} convert_8BytesToInt64;

typedef union convert_4BytesToInt32{
  int32_t integer;
  unsigned char byte[4];
} convert_4BytesToInt32;

struct MidiData{
  int midiCommand;
  int midiChannel;
  int midiNummber;
  int midiValue;
};

int process(jack_nframes_t nframes, void *input_port_raw){
  int i;
  jack_port_t* input_port = (jack_port_t*)input_port_raw;
  void* port_buf = jack_port_get_buffer(input_port, nframes);
  jack_midi_event_t in_event;
  jack_nframes_t event_index = 0;
  jack_nframes_t event_count = jack_midi_get_event_count(port_buf);

  if(event_count > 1)
  {
    printf("qt_jack_midi_debugger: got event(s): %d \n",event_count);
    for(i=0; i<event_count; i++)
    {
      jack_midi_event_get(&in_event, port_buf, i);
      printf("    event %d time is %d. 1st byte is 0x%x\n", i, in_event.time, *(in_event.buffer));
    }
    jack_midi_event_get(&in_event, port_buf, 0);

    for(i=0; i<nframes; i++){
      if((in_event.time == i) && (event_index < event_count)){
        // to do add events to a cue
        //atm just print out:
	size_t event_size = in_event.size;
        
        convert_8BytesToInt64 converter;
	converter.integer = 0;

	// should always be 3 bytes
	converter.byte[0] = *(in_event.buffer); 
        converter.byte[1] = *(in_event.buffer + 1);
	converter.byte[2] = *(in_event.buffer + 2);
	int64_t val = converter.integer;

        uint8_t type = in_event.buffer[0] & 0xf0;
	uint8_t channel = in_event.buffer[0] & 0xf;
	uint8_t pitch = in_event.buffer[1];
	uint8_t velocity = in_event.buffer[2];

	switch (type) {
	  case 0x90:
	    assert (event_size == 3);
	    printf (" note on  (channel %2d): pitch %3d, velocity %3d \n",channel,pitch,velocity);
	    break;
	  case 0x80:
	    assert (event_size == 3);
	    printf (" note off (channel %2d): pitch %3d, velocity %3d \n",channel,pitch,velocity);	    
	    break;
	  case 0xb0:
	    assert (event_size == 3);
	    printf (" control change (channel %2d): controller %3d, value %3d \n",channel,pitch,velocity);    
	    break;
	  default:
	    printf("unknown msg-type");
	    break;
	}
  printf("\n");
	printf("event_index = %d, rcf_midi_msgs_count = %#018" PRIx64 "\n", event_index, rcf_midi_msgs_count);  
	printf("size = %d; val = %#018" PRIx64 "\n", event_size, val);
  printf("\n");
	rcf_midi_msgs_count++;
	event_index++;
	if(event_index < event_count)
	  jack_midi_event_get(&in_event, port_buf, event_index);
      }
    }
	
  }
  return 0;
  
}

void jack_shutdown(void *arg)
{
  exit(1);
}

int main(int narg, char **args)
{
  rcf_midi_msgs_count = 0;
  jack_client_t *client;
  jack_port_t *input_port;

  if ((client = jack_client_open ("qt_jack_midi_debugger", JackNullOption, NULL)) == 0)
  {
    fprintf(stderr, "jack server not running?\n");
    return 1;
  }

  input_port = jack_port_register (client, "midi_in", JACK_DEFAULT_MIDI_TYPE, JackPortIsInput, 0);

  jack_set_process_callback (client, process, (void*)input_port);

  jack_on_shutdown (client, jack_shutdown, 0);

  if (jack_activate (client))
  {
    fprintf(stderr, "cannot activate client");
    return 1;
  }

  while(1)
  {
    sleep(1);
  }
  jack_client_close(client);
  exit (0);
}
