/* this file exists to navigate the strange building procedure of stb_image_resize2.h, which includes itself 26 times.
This is the relevant portion of the file that will simply be pasted instead of #include STBIR__HEADER_FILENAME */

#define STBIR_strs_join2( start, mid, end ) start##mid##end
#define STBIR_strs_join1( start, mid, end ) STBIR_strs_join2( start, mid, end )

#define STBIR_strs_join24( start, mid1, mid2, end ) start##mid1##mid2##end
#define STBIR_strs_join14( start, mid1, mid2, end ) STBIR_strs_join24( start, mid1, mid2, end )

#ifdef STB_IMAGE_RESIZE_DO_CODERS

#ifdef stbir__decode_suffix
#define STBIR__CODER_NAME( name ) STBIR_strs_join1( name, _, stbir__decode_suffix )
#else
#define STBIR__CODER_NAME( name ) name
#endif

#ifdef stbir__decode_swizzle
#define stbir__decode_simdf8_flip(reg) STBIR_strs_join1( STBIR_strs_join1( STBIR_strs_join1( STBIR_strs_join1( stbir__simdf8_0123to,stbir__decode_order0,stbir__decode_order1),stbir__decode_order2,stbir__decode_order3),stbir__decode_order0,stbir__decode_order1),stbir__decode_order2,stbir__decode_order3)(reg, reg)
#define stbir__decode_simdf4_flip(reg) STBIR_strs_join1( STBIR_strs_join1( stbir__simdf_0123to,stbir__decode_order0,stbir__decode_order1),stbir__decode_order2,stbir__decode_order3)(reg, reg)
#define stbir__encode_simdf8_unflip(reg) STBIR_strs_join1( STBIR_strs_join1( STBIR_strs_join1( STBIR_strs_join1( stbir__simdf8_0123to,stbir__encode_order0,stbir__encode_order1),stbir__encode_order2,stbir__encode_order3),stbir__encode_order0,stbir__encode_order1),stbir__encode_order2,stbir__encode_order3)(reg, reg)
#define stbir__encode_simdf4_unflip(reg) STBIR_strs_join1( STBIR_strs_join1( stbir__simdf_0123to,stbir__encode_order0,stbir__encode_order1),stbir__encode_order2,stbir__encode_order3)(reg, reg)
#else
#define stbir__decode_order0 0
#define stbir__decode_order1 1
#define stbir__decode_order2 2
#define stbir__decode_order3 3
#define stbir__encode_order0 0
#define stbir__encode_order1 1
#define stbir__encode_order2 2
#define stbir__encode_order3 3
#define stbir__decode_simdf8_flip(reg)
#define stbir__decode_simdf4_flip(reg)
#define stbir__encode_simdf8_unflip(reg)
#define stbir__encode_simdf4_unflip(reg)
#endif

#ifdef STBIR_SIMD8
#define stbir__encode_simdfX_unflip  stbir__encode_simdf8_unflip
#else
#define stbir__encode_simdfX_unflip  stbir__encode_simdf4_unflip
#endif

static float * STBIR__CODER_NAME( stbir__decode_uint8_linear_scaled )( float * decodep, int width_times_channels, void const * inputp )
{
  float STBIR_STREAMOUT_PTR( * ) decode = decodep;
  float * decode_end = (float*) decode + width_times_channels;
  unsigned char const * input = (unsigned char const*)inputp;

  #ifdef STBIR_SIMD
  unsigned char const * end_input_m16 = input + width_times_channels - 16;
  if ( width_times_channels >= 16 )
  {
    decode_end -= 16;
    STBIR_NO_UNROLL_LOOP_START_INF_FOR
    for(;;)
    {
      #ifdef STBIR_SIMD8
      stbir__simdi i; stbir__simdi8 o0,o1;
      stbir__simdf8 of0, of1;
      STBIR_NO_UNROLL(decode);
      stbir__simdi_load( i, input );
      stbir__simdi8_expand_u8_to_u32( o0, o1, i );
      stbir__simdi8_convert_i32_to_float( of0, o0 );
      stbir__simdi8_convert_i32_to_float( of1, o1 );
      stbir__simdf8_mult( of0, of0, STBIR_max_uint8_as_float_inverted8);
      stbir__simdf8_mult( of1, of1, STBIR_max_uint8_as_float_inverted8);
      stbir__decode_simdf8_flip( of0 );
      stbir__decode_simdf8_flip( of1 );
      stbir__simdf8_store( decode + 0, of0 );
      stbir__simdf8_store( decode + 8, of1 );
      #else
      stbir__simdi i, o0, o1, o2, o3;
      stbir__simdf of0, of1, of2, of3;
      STBIR_NO_UNROLL(decode);
      stbir__simdi_load( i, input );
      stbir__simdi_expand_u8_to_u32( o0,o1,o2,o3,i);
      stbir__simdi_convert_i32_to_float( of0, o0 );
      stbir__simdi_convert_i32_to_float( of1, o1 );
      stbir__simdi_convert_i32_to_float( of2, o2 );
      stbir__simdi_convert_i32_to_float( of3, o3 );
      stbir__simdf_mult( of0, of0, STBIR__CONSTF(STBIR_max_uint8_as_float_inverted) );
      stbir__simdf_mult( of1, of1, STBIR__CONSTF(STBIR_max_uint8_as_float_inverted) );
      stbir__simdf_mult( of2, of2, STBIR__CONSTF(STBIR_max_uint8_as_float_inverted) );
      stbir__simdf_mult( of3, of3, STBIR__CONSTF(STBIR_max_uint8_as_float_inverted) );
      stbir__decode_simdf4_flip( of0 );
      stbir__decode_simdf4_flip( of1 );
      stbir__decode_simdf4_flip( of2 );
      stbir__decode_simdf4_flip( of3 );
      stbir__simdf_store( decode + 0,  of0 );
      stbir__simdf_store( decode + 4,  of1 );
      stbir__simdf_store( decode + 8,  of2 );
      stbir__simdf_store( decode + 12, of3 );
      #endif
      decode += 16;
      input += 16;
      if ( decode <= decode_end )
        continue;
      if ( decode == ( decode_end + 16 ) )
        break;
      decode = decode_end; // backup and do last couple
      input = end_input_m16;
    }
    return decode_end + 16;
  }
  #endif

  // try to do blocks of 4 when you can
  #if stbir__coder_min_num != 3 // doesn't divide cleanly by four
  decode += 4;
  STBIR_SIMD_NO_UNROLL_LOOP_START
  while( decode <= decode_end )
  {
    STBIR_SIMD_NO_UNROLL(decode);
    decode[0-4] = ((float)(input[stbir__decode_order0])) * stbir__max_uint8_as_float_inverted;
    decode[1-4] = ((float)(input[stbir__decode_order1])) * stbir__max_uint8_as_float_inverted;
    decode[2-4] = ((float)(input[stbir__decode_order2])) * stbir__max_uint8_as_float_inverted;
    decode[3-4] = ((float)(input[stbir__decode_order3])) * stbir__max_uint8_as_float_inverted;
    decode += 4;
    input += 4;
  }
  decode -= 4;
  #endif

  // do the remnants
  #if stbir__coder_min_num < 4
  STBIR_NO_UNROLL_LOOP_START
  while( decode < decode_end )
  {
    STBIR_NO_UNROLL(decode);
    decode[0] = ((float)(input[stbir__decode_order0])) * stbir__max_uint8_as_float_inverted;
    #if stbir__coder_min_num >= 2
    decode[1] = ((float)(input[stbir__decode_order1])) * stbir__max_uint8_as_float_inverted;
    #endif
    #if stbir__coder_min_num >= 3
    decode[2] = ((float)(input[stbir__decode_order2])) * stbir__max_uint8_as_float_inverted;
    #endif
    decode += stbir__coder_min_num;
    input += stbir__coder_min_num;
  }
  #endif

  return decode_end;
}

static void STBIR__CODER_NAME( stbir__encode_uint8_linear_scaled )( void * outputp, int width_times_channels, float const * encode )
{
  unsigned char STBIR_SIMD_STREAMOUT_PTR( * ) output = (unsigned char *) outputp;
  unsigned char * end_output = ( (unsigned char *) output ) + width_times_channels;

  #ifdef STBIR_SIMD
  if ( width_times_channels >= stbir__simdfX_float_count*2 )
  {
    float const * end_encode_m8 = encode + width_times_channels - stbir__simdfX_float_count*2;
    end_output -= stbir__simdfX_float_count*2;
    STBIR_NO_UNROLL_LOOP_START_INF_FOR
    for(;;)
    {
      stbir__simdfX e0, e1;
      stbir__simdi i;
      STBIR_SIMD_NO_UNROLL(encode);
      stbir__simdfX_madd_mem( e0, STBIR_simd_point5X, STBIR_max_uint8_as_floatX, encode );
      stbir__simdfX_madd_mem( e1, STBIR_simd_point5X, STBIR_max_uint8_as_floatX, encode+stbir__simdfX_float_count );
      stbir__encode_simdfX_unflip( e0 );
      stbir__encode_simdfX_unflip( e1 );
      #ifdef STBIR_SIMD8
      stbir__simdf8_pack_to_16bytes( i, e0, e1 );
      stbir__simdi_store( output, i );
      #else
      stbir__simdf_pack_to_8bytes( i, e0, e1 );
      stbir__simdi_store2( output, i );
      #endif
      encode += stbir__simdfX_float_count*2;
      output += stbir__simdfX_float_count*2;
      if ( output <= end_output )
        continue;
      if ( output == ( end_output + stbir__simdfX_float_count*2 ) )
        break;
      output = end_output; // backup and do last couple
      encode = end_encode_m8;
    }
    return;
  }

  // try to do blocks of 4 when you can
  #if stbir__coder_min_num != 3 // doesn't divide cleanly by four
  output += 4;
  STBIR_NO_UNROLL_LOOP_START
  while( output <= end_output )
  {
    stbir__simdf e0;
    stbir__simdi i0;
    STBIR_NO_UNROLL(encode);
    stbir__simdf_load( e0, encode );
    stbir__simdf_madd( e0, STBIR__CONSTF(STBIR_simd_point5), STBIR__CONSTF(STBIR_max_uint8_as_float), e0 );
    stbir__encode_simdf4_unflip( e0 );
    stbir__simdf_pack_to_8bytes( i0, e0, e0 );  // only use first 4
    *(int*)(output-4) = stbir__simdi_to_int( i0 );
    output += 4;
    encode += 4;
  }
  output -= 4;
  #endif

  // do the remnants
  #if stbir__coder_min_num < 4
  STBIR_NO_UNROLL_LOOP_START
  while( output < end_output )
  {
    stbir__simdf e0;
    STBIR_NO_UNROLL(encode);
    stbir__simdf_madd1_mem( e0, STBIR__CONSTF(STBIR_simd_point5), STBIR__CONSTF(STBIR_max_uint8_as_float), encode+stbir__encode_order0 ); output[0] = stbir__simdf_convert_float_to_uint8( e0 );
    #if stbir__coder_min_num >= 2
    stbir__simdf_madd1_mem( e0, STBIR__CONSTF(STBIR_simd_point5), STBIR__CONSTF(STBIR_max_uint8_as_float), encode+stbir__encode_order1 ); output[1] = stbir__simdf_convert_float_to_uint8( e0 );
    #endif
    #if stbir__coder_min_num >= 3
    stbir__simdf_madd1_mem( e0, STBIR__CONSTF(STBIR_simd_point5), STBIR__CONSTF(STBIR_max_uint8_as_float), encode+stbir__encode_order2 ); output[2] = stbir__simdf_convert_float_to_uint8( e0 );
    #endif
    output += stbir__coder_min_num;
    encode += stbir__coder_min_num;
  }
  #endif

  #else

  // try to do blocks of 4 when you can
  #if stbir__coder_min_num != 3 // doesn't divide cleanly by four
  output += 4;
  while( output <= end_output )
  {
    float f;
    f = encode[stbir__encode_order0] * stbir__max_uint8_as_float + 0.5f; STBIR_CLAMP(f, 0, 255); output[0-4] = (unsigned char)f;
    f = encode[stbir__encode_order1] * stbir__max_uint8_as_float + 0.5f; STBIR_CLAMP(f, 0, 255); output[1-4] = (unsigned char)f;
    f = encode[stbir__encode_order2] * stbir__max_uint8_as_float + 0.5f; STBIR_CLAMP(f, 0, 255); output[2-4] = (unsigned char)f;
    f = encode[stbir__encode_order3] * stbir__max_uint8_as_float + 0.5f; STBIR_CLAMP(f, 0, 255); output[3-4] = (unsigned char)f;
    output += 4;
    encode += 4;
  }
  output -= 4;
  #endif

  // do the remnants
  #if stbir__coder_min_num < 4
  STBIR_NO_UNROLL_LOOP_START
  while( output < end_output )
  {
    float f;
    STBIR_NO_UNROLL(encode);
    f = encode[stbir__encode_order0] * stbir__max_uint8_as_float + 0.5f; STBIR_CLAMP(f, 0, 255); output[0] = (unsigned char)f;
    #if stbir__coder_min_num >= 2
    f = encode[stbir__encode_order1] * stbir__max_uint8_as_float + 0.5f; STBIR_CLAMP(f, 0, 255); output[1] = (unsigned char)f;
    #endif
    #if stbir__coder_min_num >= 3
    f = encode[stbir__encode_order2] * stbir__max_uint8_as_float + 0.5f; STBIR_CLAMP(f, 0, 255); output[2] = (unsigned char)f;
    #endif
    output += stbir__coder_min_num;
    encode += stbir__coder_min_num;
  }
  #endif
  #endif
}

static float * STBIR__CODER_NAME(stbir__decode_uint8_linear)( float * decodep, int width_times_channels, void const * inputp )
{
  float STBIR_STREAMOUT_PTR( * ) decode = decodep;
  float * decode_end = (float*) decode + width_times_channels;
  unsigned char const * input = (unsigned char const*)inputp;

  #ifdef STBIR_SIMD
  unsigned char const * end_input_m16 = input + width_times_channels - 16;
  if ( width_times_channels >= 16 )
  {
    decode_end -= 16;
    STBIR_NO_UNROLL_LOOP_START_INF_FOR
    for(;;)
    {
      #ifdef STBIR_SIMD8
      stbir__simdi i; stbir__simdi8 o0,o1;
      stbir__simdf8 of0, of1;
      STBIR_NO_UNROLL(decode);
      stbir__simdi_load( i, input );
      stbir__simdi8_expand_u8_to_u32( o0, o1, i );
      stbir__simdi8_convert_i32_to_float( of0, o0 );
      stbir__simdi8_convert_i32_to_float( of1, o1 );
      stbir__decode_simdf8_flip( of0 );
      stbir__decode_simdf8_flip( of1 );
      stbir__simdf8_store( decode + 0, of0 );
      stbir__simdf8_store( decode + 8, of1 );
      #else
      stbir__simdi i, o0, o1, o2, o3;
      stbir__simdf of0, of1, of2, of3;
      STBIR_NO_UNROLL(decode);
      stbir__simdi_load( i, input );
      stbir__simdi_expand_u8_to_u32( o0,o1,o2,o3,i);
      stbir__simdi_convert_i32_to_float( of0, o0 );
      stbir__simdi_convert_i32_to_float( of1, o1 );
      stbir__simdi_convert_i32_to_float( of2, o2 );
      stbir__simdi_convert_i32_to_float( of3, o3 );
      stbir__decode_simdf4_flip( of0 );
      stbir__decode_simdf4_flip( of1 );
      stbir__decode_simdf4_flip( of2 );
      stbir__decode_simdf4_flip( of3 );
      stbir__simdf_store( decode + 0,  of0 );
      stbir__simdf_store( decode + 4,  of1 );
      stbir__simdf_store( decode + 8,  of2 );
      stbir__simdf_store( decode + 12, of3 );
#endif
      decode += 16;
      input += 16;
      if ( decode <= decode_end )
        continue;
      if ( decode == ( decode_end + 16 ) )
        break;
      decode = decode_end; // backup and do last couple
      input = end_input_m16;
    }
    return decode_end + 16;
  }
  #endif

  // try to do blocks of 4 when you can
  #if stbir__coder_min_num != 3 // doesn't divide cleanly by four
  decode += 4;
  STBIR_SIMD_NO_UNROLL_LOOP_START
  while( decode <= decode_end )
  {
    STBIR_SIMD_NO_UNROLL(decode);
    decode[0-4] = ((float)(input[stbir__decode_order0]));
    decode[1-4] = ((float)(input[stbir__decode_order1]));
    decode[2-4] = ((float)(input[stbir__decode_order2]));
    decode[3-4] = ((float)(input[stbir__decode_order3]));
    decode += 4;
    input += 4;
  }
  decode -= 4;
  #endif

  // do the remnants
  #if stbir__coder_min_num < 4
  STBIR_NO_UNROLL_LOOP_START
  while( decode < decode_end )
  {
    STBIR_NO_UNROLL(decode);
    decode[0] = ((float)(input[stbir__decode_order0]));
    #if stbir__coder_min_num >= 2
    decode[1] = ((float)(input[stbir__decode_order1]));
    #endif
    #if stbir__coder_min_num >= 3
    decode[2] = ((float)(input[stbir__decode_order2]));
    #endif
    decode += stbir__coder_min_num;
    input += stbir__coder_min_num;
  }
  #endif
  return decode_end;
}

static void STBIR__CODER_NAME( stbir__encode_uint8_linear )( void * outputp, int width_times_channels, float const * encode )
{
  unsigned char STBIR_SIMD_STREAMOUT_PTR( * ) output = (unsigned char *) outputp;
  unsigned char * end_output = ( (unsigned char *) output ) + width_times_channels;

  #ifdef STBIR_SIMD
  if ( width_times_channels >= stbir__simdfX_float_count*2 )
  {
    float const * end_encode_m8 = encode + width_times_channels - stbir__simdfX_float_count*2;
    end_output -= stbir__simdfX_float_count*2;
    STBIR_SIMD_NO_UNROLL_LOOP_START_INF_FOR
    for(;;)
    {
      stbir__simdfX e0, e1;
      stbir__simdi i;
      STBIR_SIMD_NO_UNROLL(encode);
      stbir__simdfX_add_mem( e0, STBIR_simd_point5X, encode );
      stbir__simdfX_add_mem( e1, STBIR_simd_point5X, encode+stbir__simdfX_float_count );
      stbir__encode_simdfX_unflip( e0 );
      stbir__encode_simdfX_unflip( e1 );
      #ifdef STBIR_SIMD8
      stbir__simdf8_pack_to_16bytes( i, e0, e1 );
      stbir__simdi_store( output, i );
      #else
      stbir__simdf_pack_to_8bytes( i, e0, e1 );
      stbir__simdi_store2( output, i );
      #endif
      encode += stbir__simdfX_float_count*2;
      output += stbir__simdfX_float_count*2;
      if ( output <= end_output )
        continue;
      if ( output == ( end_output + stbir__simdfX_float_count*2 ) )
        break;
      output = end_output; // backup and do last couple
      encode = end_encode_m8;
    }
    return;
  }

  // try to do blocks of 4 when you can
  #if stbir__coder_min_num != 3 // doesn't divide cleanly by four
  output += 4;
  STBIR_NO_UNROLL_LOOP_START
  while( output <= end_output )
  {
    stbir__simdf e0;
    stbir__simdi i0;
    STBIR_NO_UNROLL(encode);
    stbir__simdf_load( e0, encode );
    stbir__simdf_add( e0, STBIR__CONSTF(STBIR_simd_point5), e0 );
    stbir__encode_simdf4_unflip( e0 );
    stbir__simdf_pack_to_8bytes( i0, e0, e0 );  // only use first 4
    *(int*)(output-4) = stbir__simdi_to_int( i0 );
    output += 4;
    encode += 4;
  }
  output -= 4;
  #endif

  #else

  // try to do blocks of 4 when you can
  #if stbir__coder_min_num != 3 // doesn't divide cleanly by four
  output += 4;
  while( output <= end_output )
  {
    float f;
    f = encode[stbir__encode_order0] + 0.5f; STBIR_CLAMP(f, 0, 255); output[0-4] = (unsigned char)f;
    f = encode[stbir__encode_order1] + 0.5f; STBIR_CLAMP(f, 0, 255); output[1-4] = (unsigned char)f;
    f = encode[stbir__encode_order2] + 0.5f; STBIR_CLAMP(f, 0, 255); output[2-4] = (unsigned char)f;
    f = encode[stbir__encode_order3] + 0.5f; STBIR_CLAMP(f, 0, 255); output[3-4] = (unsigned char)f;
    output += 4;
    encode += 4;
  }
  output -= 4;
  #endif

  #endif

  // do the remnants
  #if stbir__coder_min_num < 4
  STBIR_NO_UNROLL_LOOP_START
  while( output < end_output )
  {
    float f;
    STBIR_NO_UNROLL(encode);
    f = encode[stbir__encode_order0] + 0.5f; STBIR_CLAMP(f, 0, 255); output[0] = (unsigned char)f;
    #if stbir__coder_min_num >= 2
    f = encode[stbir__encode_order1] + 0.5f; STBIR_CLAMP(f, 0, 255); output[1] = (unsigned char)f;
    #endif
    #if stbir__coder_min_num >= 3
    f = encode[stbir__encode_order2] + 0.5f; STBIR_CLAMP(f, 0, 255); output[2] = (unsigned char)f;
    #endif
    output += stbir__coder_min_num;
    encode += stbir__coder_min_num;
  }
  #endif
}

static float * STBIR__CODER_NAME(stbir__decode_uint8_srgb)( float * decodep, int width_times_channels, void const * inputp )
{
  float STBIR_STREAMOUT_PTR( * ) decode = decodep;
  float * decode_end = (float*) decode + width_times_channels;
  unsigned char const * input = (unsigned char const *)inputp;

  // try to do blocks of 4 when you can
  #if stbir__coder_min_num != 3 // doesn't divide cleanly by four
  decode += 4;
  while( decode <= decode_end )
  {
    decode[0-4] = stbir__srgb_uchar_to_linear_float[ input[ stbir__decode_order0 ] ];
    decode[1-4] = stbir__srgb_uchar_to_linear_float[ input[ stbir__decode_order1 ] ];
    decode[2-4] = stbir__srgb_uchar_to_linear_float[ input[ stbir__decode_order2 ] ];
    decode[3-4] = stbir__srgb_uchar_to_linear_float[ input[ stbir__decode_order3 ] ];
    decode += 4;
    input += 4;
  }
  decode -= 4;
  #endif

  // do the remnants
  #if stbir__coder_min_num < 4
  STBIR_NO_UNROLL_LOOP_START
  while( decode < decode_end )
  {
    STBIR_NO_UNROLL(decode);
    decode[0] = stbir__srgb_uchar_to_linear_float[ input[ stbir__decode_order0 ] ];
    #if stbir__coder_min_num >= 2
    decode[1] = stbir__srgb_uchar_to_linear_float[ input[ stbir__decode_order1 ] ];
    #endif
    #if stbir__coder_min_num >= 3
    decode[2] = stbir__srgb_uchar_to_linear_float[ input[ stbir__decode_order2 ] ];
    #endif
    decode += stbir__coder_min_num;
    input += stbir__coder_min_num;
  }
  #endif
  return decode_end;
}

#define stbir__min_max_shift20( i, f ) \
    stbir__simdf_max( f, f, stbir_simdf_casti(STBIR__CONSTI( STBIR_almost_zero )) ); \
    stbir__simdf_min( f, f, stbir_simdf_casti(STBIR__CONSTI( STBIR_almost_one  )) ); \
    stbir__simdi_32shr( i, stbir_simdi_castf( f ), 20 );

#define stbir__scale_and_convert( i, f ) \
    stbir__simdf_madd( f, STBIR__CONSTF( STBIR_simd_point5 ), STBIR__CONSTF( STBIR_max_uint8_as_float ), f ); \
    stbir__simdf_max( f, f, stbir__simdf_zeroP() ); \
    stbir__simdf_min( f, f, STBIR__CONSTF( STBIR_max_uint8_as_float ) ); \
    stbir__simdf_convert_float_to_i32( i, f );

#define stbir__linear_to_srgb_finish( i, f ) \
{ \
    stbir__simdi temp;  \
    stbir__simdi_32shr( temp, stbir_simdi_castf( f ), 12 ) ; \
    stbir__simdi_and( temp, temp, STBIR__CONSTI(STBIR_mastissa_mask) ); \
    stbir__simdi_or( temp, temp, STBIR__CONSTI(STBIR_topscale) ); \
    stbir__simdi_16madd( i, i, temp ); \
    stbir__simdi_32shr( i, i, 16 ); \
}

#define stbir__simdi_table_lookup2( v0,v1, table ) \
{ \
  stbir__simdi_u32 temp0,temp1; \
  temp0.m128i_i128 = v0; \
  temp1.m128i_i128 = v1; \
  temp0.m128i_u32[0] = table[temp0.m128i_i32[0]]; temp0.m128i_u32[1] = table[temp0.m128i_i32[1]]; temp0.m128i_u32[2] = table[temp0.m128i_i32[2]]; temp0.m128i_u32[3] = table[temp0.m128i_i32[3]]; \
  temp1.m128i_u32[0] = table[temp1.m128i_i32[0]]; temp1.m128i_u32[1] = table[temp1.m128i_i32[1]]; temp1.m128i_u32[2] = table[temp1.m128i_i32[2]]; temp1.m128i_u32[3] = table[temp1.m128i_i32[3]]; \
  v0 = temp0.m128i_i128; \
  v1 = temp1.m128i_i128; \
}

#define stbir__simdi_table_lookup3( v0,v1,v2, table ) \
{ \
  stbir__simdi_u32 temp0,temp1,temp2; \
  temp0.m128i_i128 = v0; \
  temp1.m128i_i128 = v1; \
  temp2.m128i_i128 = v2; \
  temp0.m128i_u32[0] = table[temp0.m128i_i32[0]]; temp0.m128i_u32[1] = table[temp0.m128i_i32[1]]; temp0.m128i_u32[2] = table[temp0.m128i_i32[2]]; temp0.m128i_u32[3] = table[temp0.m128i_i32[3]]; \
  temp1.m128i_u32[0] = table[temp1.m128i_i32[0]]; temp1.m128i_u32[1] = table[temp1.m128i_i32[1]]; temp1.m128i_u32[2] = table[temp1.m128i_i32[2]]; temp1.m128i_u32[3] = table[temp1.m128i_i32[3]]; \
  temp2.m128i_u32[0] = table[temp2.m128i_i32[0]]; temp2.m128i_u32[1] = table[temp2.m128i_i32[1]]; temp2.m128i_u32[2] = table[temp2.m128i_i32[2]]; temp2.m128i_u32[3] = table[temp2.m128i_i32[3]]; \
  v0 = temp0.m128i_i128; \
  v1 = temp1.m128i_i128; \
  v2 = temp2.m128i_i128; \
}

#define stbir__simdi_table_lookup4( v0,v1,v2,v3, table ) \
{ \
  stbir__simdi_u32 temp0,temp1,temp2,temp3; \
  temp0.m128i_i128 = v0; \
  temp1.m128i_i128 = v1; \
  temp2.m128i_i128 = v2; \
  temp3.m128i_i128 = v3; \
  temp0.m128i_u32[0] = table[temp0.m128i_i32[0]]; temp0.m128i_u32[1] = table[temp0.m128i_i32[1]]; temp0.m128i_u32[2] = table[temp0.m128i_i32[2]]; temp0.m128i_u32[3] = table[temp0.m128i_i32[3]]; \
  temp1.m128i_u32[0] = table[temp1.m128i_i32[0]]; temp1.m128i_u32[1] = table[temp1.m128i_i32[1]]; temp1.m128i_u32[2] = table[temp1.m128i_i32[2]]; temp1.m128i_u32[3] = table[temp1.m128i_i32[3]]; \
  temp2.m128i_u32[0] = table[temp2.m128i_i32[0]]; temp2.m128i_u32[1] = table[temp2.m128i_i32[1]]; temp2.m128i_u32[2] = table[temp2.m128i_i32[2]]; temp2.m128i_u32[3] = table[temp2.m128i_i32[3]]; \
  temp3.m128i_u32[0] = table[temp3.m128i_i32[0]]; temp3.m128i_u32[1] = table[temp3.m128i_i32[1]]; temp3.m128i_u32[2] = table[temp3.m128i_i32[2]]; temp3.m128i_u32[3] = table[temp3.m128i_i32[3]]; \
  v0 = temp0.m128i_i128; \
  v1 = temp1.m128i_i128; \
  v2 = temp2.m128i_i128; \
  v3 = temp3.m128i_i128; \
}

static void STBIR__CODER_NAME( stbir__encode_uint8_srgb )( void * outputp, int width_times_channels, float const * encode )
{
  unsigned char STBIR_SIMD_STREAMOUT_PTR( * ) output = (unsigned char*) outputp;
  unsigned char * end_output = ( (unsigned char*) output ) + width_times_channels;

  #ifdef STBIR_SIMD

  if ( width_times_channels >= 16 )
  {
    float const * end_encode_m16 = encode + width_times_channels - 16;
    end_output -= 16;
    STBIR_SIMD_NO_UNROLL_LOOP_START_INF_FOR
    for(;;)
    {
      stbir__simdf f0, f1, f2, f3;
      stbir__simdi i0, i1, i2, i3;
      STBIR_SIMD_NO_UNROLL(encode);

      stbir__simdf_load4_transposed( f0, f1, f2, f3, encode );

      stbir__min_max_shift20( i0, f0 );
      stbir__min_max_shift20( i1, f1 );
      stbir__min_max_shift20( i2, f2 );
      stbir__min_max_shift20( i3, f3 );

      stbir__simdi_table_lookup4( i0, i1, i2, i3, ( fp32_to_srgb8_tab4 - (127-13)*8 ) );

      stbir__linear_to_srgb_finish( i0, f0 );
      stbir__linear_to_srgb_finish( i1, f1 );
      stbir__linear_to_srgb_finish( i2, f2 );
      stbir__linear_to_srgb_finish( i3, f3 );

      stbir__interleave_pack_and_store_16_u8( output,  STBIR_strs_join1(i, ,stbir__encode_order0), STBIR_strs_join1(i, ,stbir__encode_order1), STBIR_strs_join1(i, ,stbir__encode_order2), STBIR_strs_join1(i, ,stbir__encode_order3) );

      encode += 16;
      output += 16;
      if ( output <= end_output )
        continue;
      if ( output == ( end_output + 16 ) )
        break;
      output = end_output; // backup and do last couple
      encode = end_encode_m16;
    }
    return;
  }
  #endif

  // try to do blocks of 4 when you can
  #if stbir__coder_min_num != 3 // doesn't divide cleanly by four
  output += 4;
  STBIR_SIMD_NO_UNROLL_LOOP_START
  while ( output <= end_output )
  {
    STBIR_SIMD_NO_UNROLL(encode);

    output[0-4] = stbir__linear_to_srgb_uchar( encode[stbir__encode_order0] );
    output[1-4] = stbir__linear_to_srgb_uchar( encode[stbir__encode_order1] );
    output[2-4] = stbir__linear_to_srgb_uchar( encode[stbir__encode_order2] );
    output[3-4] = stbir__linear_to_srgb_uchar( encode[stbir__encode_order3] );

    output += 4;
    encode += 4;
  }
  output -= 4;
  #endif

  // do the remnants
  #if stbir__coder_min_num < 4
  STBIR_NO_UNROLL_LOOP_START
  while( output < end_output )
  {
    STBIR_NO_UNROLL(encode);
    output[0] = stbir__linear_to_srgb_uchar( encode[stbir__encode_order0] );
    #if stbir__coder_min_num >= 2
    output[1] = stbir__linear_to_srgb_uchar( encode[stbir__encode_order1] );
    #endif
    #if stbir__coder_min_num >= 3
    output[2] = stbir__linear_to_srgb_uchar( encode[stbir__encode_order2] );
    #endif
    output += stbir__coder_min_num;
    encode += stbir__coder_min_num;
  }
  #endif
}

#if ( stbir__coder_min_num == 4 ) || ( ( stbir__coder_min_num == 1 ) && ( !defined(stbir__decode_swizzle) ) )

static float * STBIR__CODER_NAME(stbir__decode_uint8_srgb4_linearalpha)( float * decodep, int width_times_channels, void const * inputp )
{
  float STBIR_STREAMOUT_PTR( * ) decode = decodep;
  float * decode_end = (float*) decode + width_times_channels;
  unsigned char const * input = (unsigned char const *)inputp;

  do {
    decode[0] = stbir__srgb_uchar_to_linear_float[ input[stbir__decode_order0] ];
    decode[1] = stbir__srgb_uchar_to_linear_float[ input[stbir__decode_order1] ];
    decode[2] = stbir__srgb_uchar_to_linear_float[ input[stbir__decode_order2] ];
    decode[3] = ( (float) input[stbir__decode_order3] ) * stbir__max_uint8_as_float_inverted;
    input += 4;
    decode += 4;
  } while( decode < decode_end );
  return decode_end;
}


static void STBIR__CODER_NAME( stbir__encode_uint8_srgb4_linearalpha )( void * outputp, int width_times_channels, float const * encode )
{
  unsigned char STBIR_SIMD_STREAMOUT_PTR( * ) output = (unsigned char*) outputp;
  unsigned char * end_output = ( (unsigned char*) output ) + width_times_channels;

  #ifdef STBIR_SIMD

  if ( width_times_channels >= 16 )
  {
    float const * end_encode_m16 = encode + width_times_channels - 16;
    end_output -= 16;
    STBIR_SIMD_NO_UNROLL_LOOP_START_INF_FOR
    for(;;)
    {
      stbir__simdf f0, f1, f2, f3;
      stbir__simdi i0, i1, i2, i3;

      STBIR_SIMD_NO_UNROLL(encode);
      stbir__simdf_load4_transposed( f0, f1, f2, f3, encode );

      stbir__min_max_shift20( i0, f0 );
      stbir__min_max_shift20( i1, f1 );
      stbir__min_max_shift20( i2, f2 );
      stbir__scale_and_convert( i3, f3 );

      stbir__simdi_table_lookup3( i0, i1, i2, ( fp32_to_srgb8_tab4 - (127-13)*8 ) );

      stbir__linear_to_srgb_finish( i0, f0 );
      stbir__linear_to_srgb_finish( i1, f1 );
      stbir__linear_to_srgb_finish( i2, f2 );

      stbir__interleave_pack_and_store_16_u8( output,  STBIR_strs_join1(i, ,stbir__encode_order0), STBIR_strs_join1(i, ,stbir__encode_order1), STBIR_strs_join1(i, ,stbir__encode_order2), STBIR_strs_join1(i, ,stbir__encode_order3) );

      output += 16;
      encode += 16;

      if ( output <= end_output )
        continue;
      if ( output == ( end_output + 16 ) )
        break;
      output = end_output; // backup and do last couple
      encode = end_encode_m16;
    }
    return;
  }
  #endif

  STBIR_SIMD_NO_UNROLL_LOOP_START
  do {
    float f;
    STBIR_SIMD_NO_UNROLL(encode);

    output[stbir__decode_order0] = stbir__linear_to_srgb_uchar( encode[0] );
    output[stbir__decode_order1] = stbir__linear_to_srgb_uchar( encode[1] );
    output[stbir__decode_order2] = stbir__linear_to_srgb_uchar( encode[2] );

    f = encode[3] * stbir__max_uint8_as_float + 0.5f;
    STBIR_CLAMP(f, 0, 255);
    output[stbir__decode_order3] = (unsigned char) f;

    output += 4;
    encode += 4;
  } while( output < end_output );
}

#endif

#if ( stbir__coder_min_num == 2 ) || ( ( stbir__coder_min_num == 1 ) && ( !defined(stbir__decode_swizzle) ) )

static float * STBIR__CODER_NAME(stbir__decode_uint8_srgb2_linearalpha)( float * decodep, int width_times_channels, void const * inputp )
{
  float STBIR_STREAMOUT_PTR( * ) decode = decodep;
  float * decode_end = (float*) decode + width_times_channels;
  unsigned char const * input = (unsigned char const *)inputp;

  decode += 4;
  while( decode <= decode_end )
  {
    decode[0-4] = stbir__srgb_uchar_to_linear_float[ input[stbir__decode_order0] ];
    decode[1-4] = ( (float) input[stbir__decode_order1] ) * stbir__max_uint8_as_float_inverted;
    decode[2-4] = stbir__srgb_uchar_to_linear_float[ input[stbir__decode_order0+2] ];
    decode[3-4] = ( (float) input[stbir__decode_order1+2] ) * stbir__max_uint8_as_float_inverted;
    input += 4;
    decode += 4;
  }
  decode -= 4;
  if( decode < decode_end )
  {
    decode[0] = stbir__srgb_uchar_to_linear_float[ stbir__decode_order0 ];
    decode[1] = ( (float) input[stbir__decode_order1] ) * stbir__max_uint8_as_float_inverted;
  }
  return decode_end;
}

static void STBIR__CODER_NAME( stbir__encode_uint8_srgb2_linearalpha )( void * outputp, int width_times_channels, float const * encode )
{
  unsigned char STBIR_SIMD_STREAMOUT_PTR( * ) output = (unsigned char*) outputp;
  unsigned char * end_output = ( (unsigned char*) output ) + width_times_channels;

  #ifdef STBIR_SIMD

  if ( width_times_channels >= 16 )
  {
    float const * end_encode_m16 = encode + width_times_channels - 16;
    end_output -= 16;
    STBIR_SIMD_NO_UNROLL_LOOP_START_INF_FOR
    for(;;)
    {
      stbir__simdf f0, f1, f2, f3;
      stbir__simdi i0, i1, i2, i3;

      STBIR_SIMD_NO_UNROLL(encode);
      stbir__simdf_load4_transposed( f0, f1, f2, f3, encode );

      stbir__min_max_shift20( i0, f0 );
      stbir__scale_and_convert( i1, f1 );
      stbir__min_max_shift20( i2, f2 );
      stbir__scale_and_convert( i3, f3 );

      stbir__simdi_table_lookup2( i0, i2, ( fp32_to_srgb8_tab4 - (127-13)*8 ) );

      stbir__linear_to_srgb_finish( i0, f0 );
      stbir__linear_to_srgb_finish( i2, f2 );

      stbir__interleave_pack_and_store_16_u8( output,  STBIR_strs_join1(i, ,stbir__encode_order0), STBIR_strs_join1(i, ,stbir__encode_order1), STBIR_strs_join1(i, ,stbir__encode_order2), STBIR_strs_join1(i, ,stbir__encode_order3) );

      output += 16;
      encode += 16;
      if ( output <= end_output )
        continue;
      if ( output == ( end_output + 16 ) )
        break;
      output = end_output; // backup and do last couple
      encode = end_encode_m16;
    }
    return;
  }
  #endif

  STBIR_SIMD_NO_UNROLL_LOOP_START
  do {
    float f;
    STBIR_SIMD_NO_UNROLL(encode);

    output[stbir__decode_order0] = stbir__linear_to_srgb_uchar( encode[0] );

    f = encode[1] * stbir__max_uint8_as_float + 0.5f;
    STBIR_CLAMP(f, 0, 255);
    output[stbir__decode_order1] = (unsigned char) f;

    output += 2;
    encode += 2;
  } while( output < end_output );
}

#endif

static float * STBIR__CODER_NAME(stbir__decode_uint16_linear_scaled)( float * decodep, int width_times_channels, void const * inputp )
{
  float STBIR_STREAMOUT_PTR( * ) decode = decodep;
  float * decode_end = (float*) decode + width_times_channels;
  unsigned short const * input = (unsigned short const *)inputp;

  #ifdef STBIR_SIMD
  unsigned short const * end_input_m8 = input + width_times_channels - 8;
  if ( width_times_channels >= 8 )
  {
    decode_end -= 8;
    STBIR_NO_UNROLL_LOOP_START_INF_FOR
    for(;;)
    {
      #ifdef STBIR_SIMD8
      stbir__simdi i; stbir__simdi8 o;
      stbir__simdf8 of;
      STBIR_NO_UNROLL(decode);
      stbir__simdi_load( i, input );
      stbir__simdi8_expand_u16_to_u32( o, i );
      stbir__simdi8_convert_i32_to_float( of, o );
      stbir__simdf8_mult( of, of, STBIR_max_uint16_as_float_inverted8);
      stbir__decode_simdf8_flip( of );
      stbir__simdf8_store( decode + 0, of );
      #else
      stbir__simdi i, o0, o1;
      stbir__simdf of0, of1;
      STBIR_NO_UNROLL(decode);
      stbir__simdi_load( i, input );
      stbir__simdi_expand_u16_to_u32( o0,o1,i );
      stbir__simdi_convert_i32_to_float( of0, o0 );
      stbir__simdi_convert_i32_to_float( of1, o1 );
      stbir__simdf_mult( of0, of0, STBIR__CONSTF(STBIR_max_uint16_as_float_inverted) );
      stbir__simdf_mult( of1, of1, STBIR__CONSTF(STBIR_max_uint16_as_float_inverted));
      stbir__decode_simdf4_flip( of0 );
      stbir__decode_simdf4_flip( of1 );
      stbir__simdf_store( decode + 0,  of0 );
      stbir__simdf_store( decode + 4,  of1 );
      #endif
      decode += 8;
      input += 8;
      if ( decode <= decode_end )
        continue;
      if ( decode == ( decode_end + 8 ) )
        break;
      decode = decode_end; // backup and do last couple
      input = end_input_m8;
    }
    return decode_end + 8;
  }
  #endif

  // try to do blocks of 4 when you can
  #if stbir__coder_min_num != 3 // doesn't divide cleanly by four
  decode += 4;
  STBIR_SIMD_NO_UNROLL_LOOP_START
  while( decode <= decode_end )
  {
    STBIR_SIMD_NO_UNROLL(decode);
    decode[0-4] = ((float)(input[stbir__decode_order0])) * stbir__max_uint16_as_float_inverted;
    decode[1-4] = ((float)(input[stbir__decode_order1])) * stbir__max_uint16_as_float_inverted;
    decode[2-4] = ((float)(input[stbir__decode_order2])) * stbir__max_uint16_as_float_inverted;
    decode[3-4] = ((float)(input[stbir__decode_order3])) * stbir__max_uint16_as_float_inverted;
    decode += 4;
    input += 4;
  }
  decode -= 4;
  #endif

  // do the remnants
  #if stbir__coder_min_num < 4
  STBIR_NO_UNROLL_LOOP_START
  while( decode < decode_end )
  {
    STBIR_NO_UNROLL(decode);
    decode[0] = ((float)(input[stbir__decode_order0])) * stbir__max_uint16_as_float_inverted;
    #if stbir__coder_min_num >= 2
    decode[1] = ((float)(input[stbir__decode_order1])) * stbir__max_uint16_as_float_inverted;
    #endif
    #if stbir__coder_min_num >= 3
    decode[2] = ((float)(input[stbir__decode_order2])) * stbir__max_uint16_as_float_inverted;
    #endif
    decode += stbir__coder_min_num;
    input += stbir__coder_min_num;
  }
  #endif
  return decode_end;
}


static void STBIR__CODER_NAME(stbir__encode_uint16_linear_scaled)( void * outputp, int width_times_channels, float const * encode )
{
  unsigned short STBIR_SIMD_STREAMOUT_PTR( * ) output = (unsigned short*) outputp;
  unsigned short * end_output = ( (unsigned short*) output ) + width_times_channels;

  #ifdef STBIR_SIMD
  {
    if ( width_times_channels >= stbir__simdfX_float_count*2 )
    {
      float const * end_encode_m8 = encode + width_times_channels - stbir__simdfX_float_count*2;
      end_output -= stbir__simdfX_float_count*2;
      STBIR_SIMD_NO_UNROLL_LOOP_START_INF_FOR
      for(;;)
      {
        stbir__simdfX e0, e1;
        stbir__simdiX i;
        STBIR_SIMD_NO_UNROLL(encode);
        stbir__simdfX_madd_mem( e0, STBIR_simd_point5X, STBIR_max_uint16_as_floatX, encode );
        stbir__simdfX_madd_mem( e1, STBIR_simd_point5X, STBIR_max_uint16_as_floatX, encode+stbir__simdfX_float_count );
        stbir__encode_simdfX_unflip( e0 );
        stbir__encode_simdfX_unflip( e1 );
        stbir__simdfX_pack_to_words( i, e0, e1 );
        stbir__simdiX_store( output, i );
        encode += stbir__simdfX_float_count*2;
        output += stbir__simdfX_float_count*2;
        if ( output <= end_output )
          continue;
        if ( output == ( end_output + stbir__simdfX_float_count*2 ) )
          break;
        output = end_output;     // backup and do last couple
        encode = end_encode_m8;
      }
      return;
    }
  }

  // try to do blocks of 4 when you can
  #if stbir__coder_min_num != 3 // doesn't divide cleanly by four
  output += 4;
  STBIR_NO_UNROLL_LOOP_START
  while( output <= end_output )
  {
    stbir__simdf e;
    stbir__simdi i;
    STBIR_NO_UNROLL(encode);
    stbir__simdf_load( e, encode );
    stbir__simdf_madd( e, STBIR__CONSTF(STBIR_simd_point5), STBIR__CONSTF(STBIR_max_uint16_as_float), e );
    stbir__encode_simdf4_unflip( e );
    stbir__simdf_pack_to_8words( i, e, e );  // only use first 4
    stbir__simdi_store2( output-4, i );
    output += 4;
    encode += 4;
  }
  output -= 4;
  #endif

  // do the remnants
  #if stbir__coder_min_num < 4
  STBIR_NO_UNROLL_LOOP_START
  while( output < end_output )
  {
    stbir__simdf e;
    STBIR_NO_UNROLL(encode);
    stbir__simdf_madd1_mem( e, STBIR__CONSTF(STBIR_simd_point5), STBIR__CONSTF(STBIR_max_uint16_as_float), encode+stbir__encode_order0 ); output[0] = stbir__simdf_convert_float_to_short( e );
    #if stbir__coder_min_num >= 2
    stbir__simdf_madd1_mem( e, STBIR__CONSTF(STBIR_simd_point5), STBIR__CONSTF(STBIR_max_uint16_as_float), encode+stbir__encode_order1 ); output[1] = stbir__simdf_convert_float_to_short( e );
    #endif
    #if stbir__coder_min_num >= 3
    stbir__simdf_madd1_mem( e, STBIR__CONSTF(STBIR_simd_point5), STBIR__CONSTF(STBIR_max_uint16_as_float), encode+stbir__encode_order2 ); output[2] = stbir__simdf_convert_float_to_short( e );
    #endif
    output += stbir__coder_min_num;
    encode += stbir__coder_min_num;
  }
  #endif

  #else

  // try to do blocks of 4 when you can
  #if stbir__coder_min_num != 3 // doesn't divide cleanly by four
  output += 4;
  STBIR_SIMD_NO_UNROLL_LOOP_START
  while( output <= end_output )
  {
    float f;
    STBIR_SIMD_NO_UNROLL(encode);
    f = encode[stbir__encode_order0] * stbir__max_uint16_as_float + 0.5f; STBIR_CLAMP(f, 0, 65535); output[0-4] = (unsigned short)f;
    f = encode[stbir__encode_order1] * stbir__max_uint16_as_float + 0.5f; STBIR_CLAMP(f, 0, 65535); output[1-4] = (unsigned short)f;
    f = encode[stbir__encode_order2] * stbir__max_uint16_as_float + 0.5f; STBIR_CLAMP(f, 0, 65535); output[2-4] = (unsigned short)f;
    f = encode[stbir__encode_order3] * stbir__max_uint16_as_float + 0.5f; STBIR_CLAMP(f, 0, 65535); output[3-4] = (unsigned short)f;
    output += 4;
    encode += 4;
  }
  output -= 4;
  #endif

  // do the remnants
  #if stbir__coder_min_num < 4
  STBIR_NO_UNROLL_LOOP_START
  while( output < end_output )
  {
    float f;
    STBIR_NO_UNROLL(encode);
    f = encode[stbir__encode_order0] * stbir__max_uint16_as_float + 0.5f; STBIR_CLAMP(f, 0, 65535); output[0] = (unsigned short)f;
    #if stbir__coder_min_num >= 2
    f = encode[stbir__encode_order1] * stbir__max_uint16_as_float + 0.5f; STBIR_CLAMP(f, 0, 65535); output[1] = (unsigned short)f;
    #endif
    #if stbir__coder_min_num >= 3
    f = encode[stbir__encode_order2] * stbir__max_uint16_as_float + 0.5f; STBIR_CLAMP(f, 0, 65535); output[2] = (unsigned short)f;
    #endif
    output += stbir__coder_min_num;
    encode += stbir__coder_min_num;
  }
  #endif
  #endif
}

static float * STBIR__CODER_NAME(stbir__decode_uint16_linear)( float * decodep, int width_times_channels, void const * inputp )
{
  float STBIR_STREAMOUT_PTR( * ) decode = decodep;
  float * decode_end = (float*) decode + width_times_channels;
  unsigned short const * input = (unsigned short const *)inputp;

  #ifdef STBIR_SIMD
  unsigned short const * end_input_m8 = input + width_times_channels - 8;
  if ( width_times_channels >= 8 )
  {
    decode_end -= 8;
    STBIR_NO_UNROLL_LOOP_START_INF_FOR
    for(;;)
    {
      #ifdef STBIR_SIMD8
      stbir__simdi i; stbir__simdi8 o;
      stbir__simdf8 of;
      STBIR_NO_UNROLL(decode);
      stbir__simdi_load( i, input );
      stbir__simdi8_expand_u16_to_u32( o, i );
      stbir__simdi8_convert_i32_to_float( of, o );
      stbir__decode_simdf8_flip( of );
      stbir__simdf8_store( decode + 0, of );
      #else
      stbir__simdi i, o0, o1;
      stbir__simdf of0, of1;
      STBIR_NO_UNROLL(decode);
      stbir__simdi_load( i, input );
      stbir__simdi_expand_u16_to_u32( o0, o1, i );
      stbir__simdi_convert_i32_to_float( of0, o0 );
      stbir__simdi_convert_i32_to_float( of1, o1 );
      stbir__decode_simdf4_flip( of0 );
      stbir__decode_simdf4_flip( of1 );
      stbir__simdf_store( decode + 0,  of0 );
      stbir__simdf_store( decode + 4,  of1 );
      #endif
      decode += 8;
      input += 8;
      if ( decode <= decode_end )
        continue;
      if ( decode == ( decode_end + 8 ) )
        break;
      decode = decode_end; // backup and do last couple
      input = end_input_m8;
    }
    return decode_end + 8;
  }
  #endif

  // try to do blocks of 4 when you can
  #if stbir__coder_min_num != 3 // doesn't divide cleanly by four
  decode += 4;
  STBIR_SIMD_NO_UNROLL_LOOP_START
  while( decode <= decode_end )
  {
    STBIR_SIMD_NO_UNROLL(decode);
    decode[0-4] = ((float)(input[stbir__decode_order0]));
    decode[1-4] = ((float)(input[stbir__decode_order1]));
    decode[2-4] = ((float)(input[stbir__decode_order2]));
    decode[3-4] = ((float)(input[stbir__decode_order3]));
    decode += 4;
    input += 4;
  }
  decode -= 4;
  #endif

  // do the remnants
  #if stbir__coder_min_num < 4
  STBIR_NO_UNROLL_LOOP_START
  while( decode < decode_end )
  {
    STBIR_NO_UNROLL(decode);
    decode[0] = ((float)(input[stbir__decode_order0]));
    #if stbir__coder_min_num >= 2
    decode[1] = ((float)(input[stbir__decode_order1]));
    #endif
    #if stbir__coder_min_num >= 3
    decode[2] = ((float)(input[stbir__decode_order2]));
    #endif
    decode += stbir__coder_min_num;
    input += stbir__coder_min_num;
  }
  #endif
  return decode_end;
}

static void STBIR__CODER_NAME(stbir__encode_uint16_linear)( void * outputp, int width_times_channels, float const * encode )
{
  unsigned short STBIR_SIMD_STREAMOUT_PTR( * ) output = (unsigned short*) outputp;
  unsigned short * end_output = ( (unsigned short*) output ) + width_times_channels;

  #ifdef STBIR_SIMD
  {
    if ( width_times_channels >= stbir__simdfX_float_count*2 )
    {
      float const * end_encode_m8 = encode + width_times_channels - stbir__simdfX_float_count*2;
      end_output -= stbir__simdfX_float_count*2;
      STBIR_SIMD_NO_UNROLL_LOOP_START_INF_FOR
      for(;;)
      {
        stbir__simdfX e0, e1;
        stbir__simdiX i;
        STBIR_SIMD_NO_UNROLL(encode);
        stbir__simdfX_add_mem( e0, STBIR_simd_point5X, encode );
        stbir__simdfX_add_mem( e1, STBIR_simd_point5X, encode+stbir__simdfX_float_count );
        stbir__encode_simdfX_unflip( e0 );
        stbir__encode_simdfX_unflip( e1 );
        stbir__simdfX_pack_to_words( i, e0, e1 );
        stbir__simdiX_store( output, i );
        encode += stbir__simdfX_float_count*2;
        output += stbir__simdfX_float_count*2;
        if ( output <= end_output )
          continue;
        if ( output == ( end_output + stbir__simdfX_float_count*2 ) )
          break;
        output = end_output; // backup and do last couple
        encode = end_encode_m8;
      }
      return;
    }
  }

  // try to do blocks of 4 when you can
  #if stbir__coder_min_num != 3 // doesn't divide cleanly by four
  output += 4;
  STBIR_NO_UNROLL_LOOP_START
  while( output <= end_output )
  {
    stbir__simdf e;
    stbir__simdi i;
    STBIR_NO_UNROLL(encode);
    stbir__simdf_load( e, encode );
    stbir__simdf_add( e, STBIR__CONSTF(STBIR_simd_point5), e );
    stbir__encode_simdf4_unflip( e );
    stbir__simdf_pack_to_8words( i, e, e );  // only use first 4
    stbir__simdi_store2( output-4, i );
    output += 4;
    encode += 4;
  }
  output -= 4;
  #endif

  #else

  // try to do blocks of 4 when you can
  #if  stbir__coder_min_num != 3 // doesn't divide cleanly by four
  output += 4;
  STBIR_SIMD_NO_UNROLL_LOOP_START
  while( output <= end_output )
  {
    float f;
    STBIR_SIMD_NO_UNROLL(encode);
    f = encode[stbir__encode_order0] + 0.5f; STBIR_CLAMP(f, 0, 65535); output[0-4] = (unsigned short)f;
    f = encode[stbir__encode_order1] + 0.5f; STBIR_CLAMP(f, 0, 65535); output[1-4] = (unsigned short)f;
    f = encode[stbir__encode_order2] + 0.5f; STBIR_CLAMP(f, 0, 65535); output[2-4] = (unsigned short)f;
    f = encode[stbir__encode_order3] + 0.5f; STBIR_CLAMP(f, 0, 65535); output[3-4] = (unsigned short)f;
    output += 4;
    encode += 4;
  }
  output -= 4;
  #endif

  #endif

  // do the remnants
  #if stbir__coder_min_num < 4
  STBIR_NO_UNROLL_LOOP_START
  while( output < end_output )
  {
    float f;
    STBIR_NO_UNROLL(encode);
    f = encode[stbir__encode_order0] + 0.5f; STBIR_CLAMP(f, 0, 65535); output[0] = (unsigned short)f;
    #if stbir__coder_min_num >= 2
    f = encode[stbir__encode_order1] + 0.5f; STBIR_CLAMP(f, 0, 65535); output[1] = (unsigned short)f;
    #endif
    #if stbir__coder_min_num >= 3
    f = encode[stbir__encode_order2] + 0.5f; STBIR_CLAMP(f, 0, 65535); output[2] = (unsigned short)f;
    #endif
    output += stbir__coder_min_num;
    encode += stbir__coder_min_num;
  }
  #endif
}

static float * STBIR__CODER_NAME(stbir__decode_half_float_linear)( float * decodep, int width_times_channels, void const * inputp )
{
  float STBIR_STREAMOUT_PTR( * ) decode = decodep;
  float * decode_end = (float*) decode + width_times_channels;
  stbir__FP16 const * input = (stbir__FP16 const *)inputp;

  #ifdef STBIR_SIMD
  if ( width_times_channels >= 8 )
  {
    stbir__FP16 const * end_input_m8 = input + width_times_channels - 8;
    decode_end -= 8;
    STBIR_NO_UNROLL_LOOP_START_INF_FOR
    for(;;)
    {
      STBIR_NO_UNROLL(decode);

      stbir__half_to_float_SIMD( decode, input );
      #ifdef stbir__decode_swizzle
      #ifdef STBIR_SIMD8
      {
        stbir__simdf8 of;
        stbir__simdf8_load( of, decode );
        stbir__decode_simdf8_flip( of );
        stbir__simdf8_store( decode, of );
      }
      #else
      {
        stbir__simdf of0,of1;
        stbir__simdf_load( of0, decode );
        stbir__simdf_load( of1, decode+4 );
        stbir__decode_simdf4_flip( of0 );
        stbir__decode_simdf4_flip( of1 );
        stbir__simdf_store( decode, of0 );
        stbir__simdf_store( decode+4, of1 );
      }
      #endif
      #endif
      decode += 8;
      input += 8;
      if ( decode <= decode_end )
        continue;
      if ( decode == ( decode_end + 8 ) )
        break;
      decode = decode_end; // backup and do last couple
      input = end_input_m8;
    }
    return decode_end + 8;
  }
  #endif

  // try to do blocks of 4 when you can
  #if stbir__coder_min_num != 3 // doesn't divide cleanly by four
  decode += 4;
  STBIR_SIMD_NO_UNROLL_LOOP_START
  while( decode <= decode_end )
  {
    STBIR_SIMD_NO_UNROLL(decode);
    decode[0-4] = stbir__half_to_float(input[stbir__decode_order0]);
    decode[1-4] = stbir__half_to_float(input[stbir__decode_order1]);
    decode[2-4] = stbir__half_to_float(input[stbir__decode_order2]);
    decode[3-4] = stbir__half_to_float(input[stbir__decode_order3]);
    decode += 4;
    input += 4;
  }
  decode -= 4;
  #endif

  // do the remnants
  #if stbir__coder_min_num < 4
  STBIR_NO_UNROLL_LOOP_START
  while( decode < decode_end )
  {
    STBIR_NO_UNROLL(decode);
    decode[0] = stbir__half_to_float(input[stbir__decode_order0]);
    #if stbir__coder_min_num >= 2
    decode[1] = stbir__half_to_float(input[stbir__decode_order1]);
    #endif
    #if stbir__coder_min_num >= 3
    decode[2] = stbir__half_to_float(input[stbir__decode_order2]);
    #endif
    decode += stbir__coder_min_num;
    input += stbir__coder_min_num;
  }
  #endif
  return decode_end;
}

static void STBIR__CODER_NAME( stbir__encode_half_float_linear )( void * outputp, int width_times_channels, float const * encode )
{
  stbir__FP16 STBIR_SIMD_STREAMOUT_PTR( * ) output = (stbir__FP16*) outputp;
  stbir__FP16 * end_output = ( (stbir__FP16*) output ) + width_times_channels;

  #ifdef STBIR_SIMD
  if ( width_times_channels >= 8 )
  {
    float const * end_encode_m8 = encode + width_times_channels - 8;
    end_output -= 8;
    STBIR_SIMD_NO_UNROLL_LOOP_START_INF_FOR
    for(;;)
    {
      STBIR_SIMD_NO_UNROLL(encode);
      #ifdef stbir__decode_swizzle
      #ifdef STBIR_SIMD8
      {
        stbir__simdf8 of;
        stbir__simdf8_load( of, encode );
        stbir__encode_simdf8_unflip( of );
        stbir__float_to_half_SIMD( output, (float*)&of );
      }
      #else
      {
        stbir__simdf of[2];
        stbir__simdf_load( of[0], encode );
        stbir__simdf_load( of[1], encode+4 );
        stbir__encode_simdf4_unflip( of[0] );
        stbir__encode_simdf4_unflip( of[1] );
        stbir__float_to_half_SIMD( output, (float*)of );
      }
      #endif
      #else
      stbir__float_to_half_SIMD( output, encode );
      #endif
      encode += 8;
      output += 8;
      if ( output <= end_output )
        continue;
      if ( output == ( end_output + 8 ) )
        break;
      output = end_output; // backup and do last couple
      encode = end_encode_m8;
    }
    return;
  }
  #endif

  // try to do blocks of 4 when you can
  #if stbir__coder_min_num != 3 // doesn't divide cleanly by four
  output += 4;
  STBIR_SIMD_NO_UNROLL_LOOP_START
  while( output <= end_output )
  {
    STBIR_SIMD_NO_UNROLL(output);
    output[0-4] = stbir__float_to_half(encode[stbir__encode_order0]);
    output[1-4] = stbir__float_to_half(encode[stbir__encode_order1]);
    output[2-4] = stbir__float_to_half(encode[stbir__encode_order2]);
    output[3-4] = stbir__float_to_half(encode[stbir__encode_order3]);
    output += 4;
    encode += 4;
  }
  output -= 4;
  #endif

  // do the remnants
  #if stbir__coder_min_num < 4
  STBIR_NO_UNROLL_LOOP_START
  while( output < end_output )
  {
    STBIR_NO_UNROLL(output);
    output[0] = stbir__float_to_half(encode[stbir__encode_order0]);
    #if stbir__coder_min_num >= 2
    output[1] = stbir__float_to_half(encode[stbir__encode_order1]);
    #endif
    #if stbir__coder_min_num >= 3
    output[2] = stbir__float_to_half(encode[stbir__encode_order2]);
    #endif
    output += stbir__coder_min_num;
    encode += stbir__coder_min_num;
  }
  #endif
}

static float * STBIR__CODER_NAME(stbir__decode_float_linear)( float * decodep, int width_times_channels, void const * inputp )
{
  #ifdef stbir__decode_swizzle
  float STBIR_STREAMOUT_PTR( * ) decode = decodep;
  float * decode_end = (float*) decode + width_times_channels;
  float const * input = (float const *)inputp;

  #ifdef STBIR_SIMD
  if ( width_times_channels >= 16 )
  {
    float const * end_input_m16 = input + width_times_channels - 16;
    decode_end -= 16;
    STBIR_NO_UNROLL_LOOP_START_INF_FOR
    for(;;)
    {
      STBIR_NO_UNROLL(decode);
      #ifdef stbir__decode_swizzle
      #ifdef STBIR_SIMD8
      {
        stbir__simdf8 of0,of1;
        stbir__simdf8_load( of0, input );
        stbir__simdf8_load( of1, input+8 );
        stbir__decode_simdf8_flip( of0 );
        stbir__decode_simdf8_flip( of1 );
        stbir__simdf8_store( decode, of0 );
        stbir__simdf8_store( decode+8, of1 );
      }
      #else
      {
        stbir__simdf of0,of1,of2,of3;
        stbir__simdf_load( of0, input );
        stbir__simdf_load( of1, input+4 );
        stbir__simdf_load( of2, input+8 );
        stbir__simdf_load( of3, input+12 );
        stbir__decode_simdf4_flip( of0 );
        stbir__decode_simdf4_flip( of1 );
        stbir__decode_simdf4_flip( of2 );
        stbir__decode_simdf4_flip( of3 );
        stbir__simdf_store( decode, of0 );
        stbir__simdf_store( decode+4, of1 );
        stbir__simdf_store( decode+8, of2 );
        stbir__simdf_store( decode+12, of3 );
      }
      #endif
      #endif
      decode += 16;
      input += 16;
      if ( decode <= decode_end )
        continue;
      if ( decode == ( decode_end + 16 ) )
        break;
      decode = decode_end; // backup and do last couple
      input = end_input_m16;
    }
    return decode_end + 16;
  }
  #endif

  // try to do blocks of 4 when you can
  #if stbir__coder_min_num != 3 // doesn't divide cleanly by four
  decode += 4;
  STBIR_SIMD_NO_UNROLL_LOOP_START
  while( decode <= decode_end )
  {
    STBIR_SIMD_NO_UNROLL(decode);
    decode[0-4] = input[stbir__decode_order0];
    decode[1-4] = input[stbir__decode_order1];
    decode[2-4] = input[stbir__decode_order2];
    decode[3-4] = input[stbir__decode_order3];
    decode += 4;
    input += 4;
  }
  decode -= 4;
  #endif

  // do the remnants
  #if stbir__coder_min_num < 4
  STBIR_NO_UNROLL_LOOP_START
  while( decode < decode_end )
  {
    STBIR_NO_UNROLL(decode);
    decode[0] = input[stbir__decode_order0];
    #if stbir__coder_min_num >= 2
    decode[1] = input[stbir__decode_order1];
    #endif
    #if stbir__coder_min_num >= 3
    decode[2] = input[stbir__decode_order2];
    #endif
    decode += stbir__coder_min_num;
    input += stbir__coder_min_num;
  }
  #endif
  return decode_end;

  #else

  if ( (void*)decodep != inputp )
    STBIR_MEMCPY( decodep, inputp, width_times_channels * sizeof( float ) );

  return decodep + width_times_channels;

  #endif
}

static void STBIR__CODER_NAME( stbir__encode_float_linear )( void * outputp, int width_times_channels, float const * encode )
{
  #if !defined( STBIR_FLOAT_HIGH_CLAMP ) && !defined(STBIR_FLOAT_LO_CLAMP) && !defined(stbir__decode_swizzle)

  if ( (void*)outputp != (void*) encode )
    STBIR_MEMCPY( outputp, encode, width_times_channels * sizeof( float ) );

  #else

  float STBIR_SIMD_STREAMOUT_PTR( * ) output = (float*) outputp;
  float * end_output = ( (float*) output ) + width_times_channels;

  #ifdef STBIR_FLOAT_HIGH_CLAMP
  #define stbir_scalar_hi_clamp( v ) if ( v > STBIR_FLOAT_HIGH_CLAMP ) v = STBIR_FLOAT_HIGH_CLAMP;
  #else
  #define stbir_scalar_hi_clamp( v )
  #endif
  #ifdef STBIR_FLOAT_LOW_CLAMP
  #define stbir_scalar_lo_clamp( v ) if ( v < STBIR_FLOAT_LOW_CLAMP ) v = STBIR_FLOAT_LOW_CLAMP;
  #else
  #define stbir_scalar_lo_clamp( v )
  #endif

  #ifdef STBIR_SIMD

  #ifdef STBIR_FLOAT_HIGH_CLAMP
  const stbir__simdfX high_clamp = stbir__simdf_frepX(STBIR_FLOAT_HIGH_CLAMP);
  #endif
  #ifdef STBIR_FLOAT_LOW_CLAMP
  const stbir__simdfX low_clamp = stbir__simdf_frepX(STBIR_FLOAT_LOW_CLAMP);
  #endif

  if ( width_times_channels >= ( stbir__simdfX_float_count * 2 ) )
  {
    float const * end_encode_m8 = encode + width_times_channels - ( stbir__simdfX_float_count * 2 );
    end_output -= ( stbir__simdfX_float_count * 2 );
    STBIR_SIMD_NO_UNROLL_LOOP_START_INF_FOR
    for(;;)
    {
      stbir__simdfX e0, e1;
      STBIR_SIMD_NO_UNROLL(encode);
      stbir__simdfX_load( e0, encode );
      stbir__simdfX_load( e1, encode+stbir__simdfX_float_count );
#ifdef STBIR_FLOAT_HIGH_CLAMP
      stbir__simdfX_min( e0, e0, high_clamp );
      stbir__simdfX_min( e1, e1, high_clamp );
#endif
#ifdef STBIR_FLOAT_LOW_CLAMP
      stbir__simdfX_max( e0, e0, low_clamp );
      stbir__simdfX_max( e1, e1, low_clamp );
#endif
      stbir__encode_simdfX_unflip( e0 );
      stbir__encode_simdfX_unflip( e1 );
      stbir__simdfX_store( output, e0 );
      stbir__simdfX_store( output+stbir__simdfX_float_count, e1 );
      encode += stbir__simdfX_float_count * 2;
      output += stbir__simdfX_float_count * 2;
      if ( output < end_output )
        continue;
      if ( output == ( end_output + ( stbir__simdfX_float_count * 2 ) ) )
        break;
      output = end_output; // backup and do last couple
      encode = end_encode_m8;
    }
    return;
  }

  // try to do blocks of 4 when you can
  #if stbir__coder_min_num != 3 // doesn't divide cleanly by four
  output += 4;
  STBIR_NO_UNROLL_LOOP_START
  while( output <= end_output )
  {
    stbir__simdf e0;
    STBIR_NO_UNROLL(encode);
    stbir__simdf_load( e0, encode );
#ifdef STBIR_FLOAT_HIGH_CLAMP
    stbir__simdf_min( e0, e0, high_clamp );
#endif
#ifdef STBIR_FLOAT_LOW_CLAMP
    stbir__simdf_max( e0, e0, low_clamp );
#endif
    stbir__encode_simdf4_unflip( e0 );
    stbir__simdf_store( output-4, e0 );
    output += 4;
    encode += 4;
  }
  output -= 4;
  #endif

  #else

  // try to do blocks of 4 when you can
  #if stbir__coder_min_num != 3 // doesn't divide cleanly by four
  output += 4;
  STBIR_SIMD_NO_UNROLL_LOOP_START
  while( output <= end_output )
  {
    float e;
    STBIR_SIMD_NO_UNROLL(encode);
    e = encode[ stbir__encode_order0 ]; stbir_scalar_hi_clamp( e ); stbir_scalar_lo_clamp( e ); output[0-4] = e;
    e = encode[ stbir__encode_order1 ]; stbir_scalar_hi_clamp( e ); stbir_scalar_lo_clamp( e ); output[1-4] = e;
    e = encode[ stbir__encode_order2 ]; stbir_scalar_hi_clamp( e ); stbir_scalar_lo_clamp( e ); output[2-4] = e;
    e = encode[ stbir__encode_order3 ]; stbir_scalar_hi_clamp( e ); stbir_scalar_lo_clamp( e ); output[3-4] = e;
    output += 4;
    encode += 4;
  }
  output -= 4;

  #endif

  #endif

  // do the remnants
  #if stbir__coder_min_num < 4
  STBIR_NO_UNROLL_LOOP_START
  while( output < end_output )
  {
    float e;
    STBIR_NO_UNROLL(encode);
    e = encode[ stbir__encode_order0 ]; stbir_scalar_hi_clamp( e ); stbir_scalar_lo_clamp( e ); output[0] = e;
    #if stbir__coder_min_num >= 2
    e = encode[ stbir__encode_order1 ]; stbir_scalar_hi_clamp( e ); stbir_scalar_lo_clamp( e ); output[1] = e;
    #endif
    #if stbir__coder_min_num >= 3
    e = encode[ stbir__encode_order2 ]; stbir_scalar_hi_clamp( e ); stbir_scalar_lo_clamp( e ); output[2] = e;
    #endif
    output += stbir__coder_min_num;
    encode += stbir__coder_min_num;
  }
  #endif

  #endif
}

#undef stbir__decode_suffix
#undef stbir__decode_simdf8_flip
#undef stbir__decode_simdf4_flip
#undef stbir__decode_order0
#undef stbir__decode_order1
#undef stbir__decode_order2
#undef stbir__decode_order3
#undef stbir__encode_order0
#undef stbir__encode_order1
#undef stbir__encode_order2
#undef stbir__encode_order3
#undef stbir__encode_simdf8_unflip
#undef stbir__encode_simdf4_unflip
#undef stbir__encode_simdfX_unflip
#undef STBIR__CODER_NAME
#undef stbir__coder_min_num
#undef stbir__decode_swizzle
#undef stbir_scalar_hi_clamp
#undef stbir_scalar_lo_clamp
#undef STB_IMAGE_RESIZE_DO_CODERS

#elif defined( STB_IMAGE_RESIZE_DO_VERTICALS)

#ifdef STB_IMAGE_RESIZE_VERTICAL_CONTINUE
#define STBIR_chans( start, end ) STBIR_strs_join14(start,STBIR__vertical_channels,end,_cont)
#else
#define STBIR_chans( start, end ) STBIR_strs_join1(start,STBIR__vertical_channels,end)
#endif

#if STBIR__vertical_channels >= 1
#define stbIF0( code ) code
#else
#define stbIF0( code )
#endif
#if STBIR__vertical_channels >= 2
#define stbIF1( code ) code
#else
#define stbIF1( code )
#endif
#if STBIR__vertical_channels >= 3
#define stbIF2( code ) code
#else
#define stbIF2( code )
#endif
#if STBIR__vertical_channels >= 4
#define stbIF3( code ) code
#else
#define stbIF3( code )
#endif
#if STBIR__vertical_channels >= 5
#define stbIF4( code ) code
#else
#define stbIF4( code )
#endif
#if STBIR__vertical_channels >= 6
#define stbIF5( code ) code
#else
#define stbIF5( code )
#endif
#if STBIR__vertical_channels >= 7
#define stbIF6( code ) code
#else
#define stbIF6( code )
#endif
#if STBIR__vertical_channels >= 8
#define stbIF7( code ) code
#else
#define stbIF7( code )
#endif

static void STBIR_chans( stbir__vertical_scatter_with_,_coeffs)( float ** outputs, float const * vertical_coefficients, float const * input, float const * input_end )
{
  stbIF0( float STBIR_SIMD_STREAMOUT_PTR( * ) output0 = outputs[0]; float c0s = vertical_coefficients[0]; )
  stbIF1( float STBIR_SIMD_STREAMOUT_PTR( * ) output1 = outputs[1]; float c1s = vertical_coefficients[1]; )
  stbIF2( float STBIR_SIMD_STREAMOUT_PTR( * ) output2 = outputs[2]; float c2s = vertical_coefficients[2]; )
  stbIF3( float STBIR_SIMD_STREAMOUT_PTR( * ) output3 = outputs[3]; float c3s = vertical_coefficients[3]; )
  stbIF4( float STBIR_SIMD_STREAMOUT_PTR( * ) output4 = outputs[4]; float c4s = vertical_coefficients[4]; )
  stbIF5( float STBIR_SIMD_STREAMOUT_PTR( * ) output5 = outputs[5]; float c5s = vertical_coefficients[5]; )
  stbIF6( float STBIR_SIMD_STREAMOUT_PTR( * ) output6 = outputs[6]; float c6s = vertical_coefficients[6]; )
  stbIF7( float STBIR_SIMD_STREAMOUT_PTR( * ) output7 = outputs[7]; float c7s = vertical_coefficients[7]; )

  #ifdef STBIR_SIMD
  {
    stbIF0(stbir__simdfX c0 = stbir__simdf_frepX( c0s ); )
    stbIF1(stbir__simdfX c1 = stbir__simdf_frepX( c1s ); )
    stbIF2(stbir__simdfX c2 = stbir__simdf_frepX( c2s ); )
    stbIF3(stbir__simdfX c3 = stbir__simdf_frepX( c3s ); )
    stbIF4(stbir__simdfX c4 = stbir__simdf_frepX( c4s ); )
    stbIF5(stbir__simdfX c5 = stbir__simdf_frepX( c5s ); )
    stbIF6(stbir__simdfX c6 = stbir__simdf_frepX( c6s ); )
    stbIF7(stbir__simdfX c7 = stbir__simdf_frepX( c7s ); )
    STBIR_SIMD_NO_UNROLL_LOOP_START
    while ( ( (char*)input_end - (char*) input ) >= (16*stbir__simdfX_float_count) )
    {
      stbir__simdfX o0, o1, o2, o3, r0, r1, r2, r3;
      STBIR_SIMD_NO_UNROLL(output0);

      stbir__simdfX_load( r0, input );               stbir__simdfX_load( r1, input+stbir__simdfX_float_count );     stbir__simdfX_load( r2, input+(2*stbir__simdfX_float_count) );      stbir__simdfX_load( r3, input+(3*stbir__simdfX_float_count) );

      #ifdef STB_IMAGE_RESIZE_VERTICAL_CONTINUE
      stbIF0( stbir__simdfX_load( o0, output0 );     stbir__simdfX_load( o1, output0+stbir__simdfX_float_count );   stbir__simdfX_load( o2, output0+(2*stbir__simdfX_float_count) );    stbir__simdfX_load( o3, output0+(3*stbir__simdfX_float_count) );
              stbir__simdfX_madd( o0, o0, r0, c0 );  stbir__simdfX_madd( o1, o1, r1, c0 );  stbir__simdfX_madd( o2, o2, r2, c0 );   stbir__simdfX_madd( o3, o3, r3, c0 );
              stbir__simdfX_store( output0, o0 );    stbir__simdfX_store( output0+stbir__simdfX_float_count, o1 );  stbir__simdfX_store( output0+(2*stbir__simdfX_float_count), o2 );   stbir__simdfX_store( output0+(3*stbir__simdfX_float_count), o3 ); )
      stbIF1( stbir__simdfX_load( o0, output1 );     stbir__simdfX_load( o1, output1+stbir__simdfX_float_count );   stbir__simdfX_load( o2, output1+(2*stbir__simdfX_float_count) );    stbir__simdfX_load( o3, output1+(3*stbir__simdfX_float_count) );
              stbir__simdfX_madd( o0, o0, r0, c1 );  stbir__simdfX_madd( o1, o1, r1, c1 );  stbir__simdfX_madd( o2, o2, r2, c1 );   stbir__simdfX_madd( o3, o3, r3, c1 );
              stbir__simdfX_store( output1, o0 );    stbir__simdfX_store( output1+stbir__simdfX_float_count, o1 );  stbir__simdfX_store( output1+(2*stbir__simdfX_float_count), o2 );   stbir__simdfX_store( output1+(3*stbir__simdfX_float_count), o3 ); )
      stbIF2( stbir__simdfX_load( o0, output2 );     stbir__simdfX_load( o1, output2+stbir__simdfX_float_count );   stbir__simdfX_load( o2, output2+(2*stbir__simdfX_float_count) );    stbir__simdfX_load( o3, output2+(3*stbir__simdfX_float_count) );
              stbir__simdfX_madd( o0, o0, r0, c2 );  stbir__simdfX_madd( o1, o1, r1, c2 );  stbir__simdfX_madd( o2, o2, r2, c2 );   stbir__simdfX_madd( o3, o3, r3, c2 );
              stbir__simdfX_store( output2, o0 );    stbir__simdfX_store( output2+stbir__simdfX_float_count, o1 );  stbir__simdfX_store( output2+(2*stbir__simdfX_float_count), o2 );   stbir__simdfX_store( output2+(3*stbir__simdfX_float_count), o3 ); )
      stbIF3( stbir__simdfX_load( o0, output3 );     stbir__simdfX_load( o1, output3+stbir__simdfX_float_count );   stbir__simdfX_load( o2, output3+(2*stbir__simdfX_float_count) );    stbir__simdfX_load( o3, output3+(3*stbir__simdfX_float_count) );
              stbir__simdfX_madd( o0, o0, r0, c3 );  stbir__simdfX_madd( o1, o1, r1, c3 );  stbir__simdfX_madd( o2, o2, r2, c3 );   stbir__simdfX_madd( o3, o3, r3, c3 );
              stbir__simdfX_store( output3, o0 );    stbir__simdfX_store( output3+stbir__simdfX_float_count, o1 );  stbir__simdfX_store( output3+(2*stbir__simdfX_float_count), o2 );   stbir__simdfX_store( output3+(3*stbir__simdfX_float_count), o3 ); )
      stbIF4( stbir__simdfX_load( o0, output4 );     stbir__simdfX_load( o1, output4+stbir__simdfX_float_count );   stbir__simdfX_load( o2, output4+(2*stbir__simdfX_float_count) );    stbir__simdfX_load( o3, output4+(3*stbir__simdfX_float_count) );
              stbir__simdfX_madd( o0, o0, r0, c4 );  stbir__simdfX_madd( o1, o1, r1, c4 );  stbir__simdfX_madd( o2, o2, r2, c4 );   stbir__simdfX_madd( o3, o3, r3, c4 );
              stbir__simdfX_store( output4, o0 );    stbir__simdfX_store( output4+stbir__simdfX_float_count, o1 );  stbir__simdfX_store( output4+(2*stbir__simdfX_float_count), o2 );   stbir__simdfX_store( output4+(3*stbir__simdfX_float_count), o3 ); )
      stbIF5( stbir__simdfX_load( o0, output5 );     stbir__simdfX_load( o1, output5+stbir__simdfX_float_count );   stbir__simdfX_load( o2, output5+(2*stbir__simdfX_float_count));    stbir__simdfX_load( o3, output5+(3*stbir__simdfX_float_count) );
              stbir__simdfX_madd( o0, o0, r0, c5 );  stbir__simdfX_madd( o1, o1, r1, c5 );  stbir__simdfX_madd( o2, o2, r2, c5 );   stbir__simdfX_madd( o3, o3, r3, c5 );
              stbir__simdfX_store( output5, o0 );    stbir__simdfX_store( output5+stbir__simdfX_float_count, o1 );  stbir__simdfX_store( output5+(2*stbir__simdfX_float_count), o2 );   stbir__simdfX_store( output5+(3*stbir__simdfX_float_count), o3 ); )
      stbIF6( stbir__simdfX_load( o0, output6 );     stbir__simdfX_load( o1, output6+stbir__simdfX_float_count );   stbir__simdfX_load( o2, output6+(2*stbir__simdfX_float_count) );    stbir__simdfX_load( o3, output6+(3*stbir__simdfX_float_count) );
              stbir__simdfX_madd( o0, o0, r0, c6 );  stbir__simdfX_madd( o1, o1, r1, c6 );  stbir__simdfX_madd( o2, o2, r2, c6 );   stbir__simdfX_madd( o3, o3, r3, c6 );
              stbir__simdfX_store( output6, o0 );    stbir__simdfX_store( output6+stbir__simdfX_float_count, o1 );  stbir__simdfX_store( output6+(2*stbir__simdfX_float_count), o2 );   stbir__simdfX_store( output6+(3*stbir__simdfX_float_count), o3 ); )
      stbIF7( stbir__simdfX_load( o0, output7 );     stbir__simdfX_load( o1, output7+stbir__simdfX_float_count );   stbir__simdfX_load( o2, output7+(2*stbir__simdfX_float_count) );    stbir__simdfX_load( o3, output7+(3*stbir__simdfX_float_count) );
              stbir__simdfX_madd( o0, o0, r0, c7 );  stbir__simdfX_madd( o1, o1, r1, c7 );  stbir__simdfX_madd( o2, o2, r2, c7 );   stbir__simdfX_madd( o3, o3, r3, c7 );
              stbir__simdfX_store( output7, o0 );    stbir__simdfX_store( output7+stbir__simdfX_float_count, o1 );  stbir__simdfX_store( output7+(2*stbir__simdfX_float_count), o2 );   stbir__simdfX_store( output7+(3*stbir__simdfX_float_count), o3 ); )
      #else
      stbIF0( stbir__simdfX_mult( o0, r0, c0 );      stbir__simdfX_mult( o1, r1, c0 );      stbir__simdfX_mult( o2, r2, c0 );       stbir__simdfX_mult( o3, r3, c0 );
              stbir__simdfX_store( output0, o0 );    stbir__simdfX_store( output0+stbir__simdfX_float_count, o1 );  stbir__simdfX_store( output0+(2*stbir__simdfX_float_count), o2 );   stbir__simdfX_store( output0+(3*stbir__simdfX_float_count), o3 ); )
      stbIF1( stbir__simdfX_mult( o0, r0, c1 );      stbir__simdfX_mult( o1, r1, c1 );      stbir__simdfX_mult( o2, r2, c1 );       stbir__simdfX_mult( o3, r3, c1 );
              stbir__simdfX_store( output1, o0 );    stbir__simdfX_store( output1+stbir__simdfX_float_count, o1 );  stbir__simdfX_store( output1+(2*stbir__simdfX_float_count), o2 );   stbir__simdfX_store( output1+(3*stbir__simdfX_float_count), o3 ); )
      stbIF2( stbir__simdfX_mult( o0, r0, c2 );      stbir__simdfX_mult( o1, r1, c2 );      stbir__simdfX_mult( o2, r2, c2 );       stbir__simdfX_mult( o3, r3, c2 );
              stbir__simdfX_store( output2, o0 );    stbir__simdfX_store( output2+stbir__simdfX_float_count, o1 );  stbir__simdfX_store( output2+(2*stbir__simdfX_float_count), o2 );   stbir__simdfX_store( output2+(3*stbir__simdfX_float_count), o3 ); )
      stbIF3( stbir__simdfX_mult( o0, r0, c3 );      stbir__simdfX_mult( o1, r1, c3 );      stbir__simdfX_mult( o2, r2, c3 );       stbir__simdfX_mult( o3, r3, c3 );
              stbir__simdfX_store( output3, o0 );    stbir__simdfX_store( output3+stbir__simdfX_float_count, o1 );  stbir__simdfX_store( output3+(2*stbir__simdfX_float_count), o2 );   stbir__simdfX_store( output3+(3*stbir__simdfX_float_count), o3 ); )
      stbIF4( stbir__simdfX_mult( o0, r0, c4 );      stbir__simdfX_mult( o1, r1, c4 );      stbir__simdfX_mult( o2, r2, c4 );       stbir__simdfX_mult( o3, r3, c4 );
              stbir__simdfX_store( output4, o0 );    stbir__simdfX_store( output4+stbir__simdfX_float_count, o1 );  stbir__simdfX_store( output4+(2*stbir__simdfX_float_count), o2 );   stbir__simdfX_store( output4+(3*stbir__simdfX_float_count), o3 ); )
      stbIF5( stbir__simdfX_mult( o0, r0, c5 );      stbir__simdfX_mult( o1, r1, c5 );      stbir__simdfX_mult( o2, r2, c5 );       stbir__simdfX_mult( o3, r3, c5 );
              stbir__simdfX_store( output5, o0 );    stbir__simdfX_store( output5+stbir__simdfX_float_count, o1 );  stbir__simdfX_store( output5+(2*stbir__simdfX_float_count), o2 );   stbir__simdfX_store( output5+(3*stbir__simdfX_float_count), o3 ); )
      stbIF6( stbir__simdfX_mult( o0, r0, c6 );      stbir__simdfX_mult( o1, r1, c6 );      stbir__simdfX_mult( o2, r2, c6 );       stbir__simdfX_mult( o3, r3, c6 );
              stbir__simdfX_store( output6, o0 );    stbir__simdfX_store( output6+stbir__simdfX_float_count, o1 );  stbir__simdfX_store( output6+(2*stbir__simdfX_float_count), o2 );   stbir__simdfX_store( output6+(3*stbir__simdfX_float_count), o3 ); )
      stbIF7( stbir__simdfX_mult( o0, r0, c7 );      stbir__simdfX_mult( o1, r1, c7 );      stbir__simdfX_mult( o2, r2, c7 );       stbir__simdfX_mult( o3, r3, c7 );
              stbir__simdfX_store( output7, o0 );    stbir__simdfX_store( output7+stbir__simdfX_float_count, o1 );  stbir__simdfX_store( output7+(2*stbir__simdfX_float_count), o2 );   stbir__simdfX_store( output7+(3*stbir__simdfX_float_count), o3 ); )
      #endif

      input += (4*stbir__simdfX_float_count);
      stbIF0( output0 += (4*stbir__simdfX_float_count); ) stbIF1( output1 += (4*stbir__simdfX_float_count); ) stbIF2( output2 += (4*stbir__simdfX_float_count); ) stbIF3( output3 += (4*stbir__simdfX_float_count); ) stbIF4( output4 += (4*stbir__simdfX_float_count); ) stbIF5( output5 += (4*stbir__simdfX_float_count); ) stbIF6( output6 += (4*stbir__simdfX_float_count); ) stbIF7( output7 += (4*stbir__simdfX_float_count); )
    }
    STBIR_SIMD_NO_UNROLL_LOOP_START
    while ( ( (char*)input_end - (char*) input ) >= 16 )
    {
      stbir__simdf o0, r0;
      STBIR_SIMD_NO_UNROLL(output0);

      stbir__simdf_load( r0, input );

      #ifdef STB_IMAGE_RESIZE_VERTICAL_CONTINUE
      stbIF0( stbir__simdf_load( o0, output0 );  stbir__simdf_madd( o0, o0, r0, stbir__if_simdf8_cast_to_simdf4( c0 ) );  stbir__simdf_store( output0, o0 ); )
      stbIF1( stbir__simdf_load( o0, output1 );  stbir__simdf_madd( o0, o0, r0, stbir__if_simdf8_cast_to_simdf4( c1 ) );  stbir__simdf_store( output1, o0 ); )
      stbIF2( stbir__simdf_load( o0, output2 );  stbir__simdf_madd( o0, o0, r0, stbir__if_simdf8_cast_to_simdf4( c2 ) );  stbir__simdf_store( output2, o0 ); )
      stbIF3( stbir__simdf_load( o0, output3 );  stbir__simdf_madd( o0, o0, r0, stbir__if_simdf8_cast_to_simdf4( c3 ) );  stbir__simdf_store( output3, o0 ); )
      stbIF4( stbir__simdf_load( o0, output4 );  stbir__simdf_madd( o0, o0, r0, stbir__if_simdf8_cast_to_simdf4( c4 ) );  stbir__simdf_store( output4, o0 ); )
      stbIF5( stbir__simdf_load( o0, output5 );  stbir__simdf_madd( o0, o0, r0, stbir__if_simdf8_cast_to_simdf4( c5 ) );  stbir__simdf_store( output5, o0 ); )
      stbIF6( stbir__simdf_load( o0, output6 );  stbir__simdf_madd( o0, o0, r0, stbir__if_simdf8_cast_to_simdf4( c6 ) );  stbir__simdf_store( output6, o0 ); )
      stbIF7( stbir__simdf_load( o0, output7 );  stbir__simdf_madd( o0, o0, r0, stbir__if_simdf8_cast_to_simdf4( c7 ) );  stbir__simdf_store( output7, o0 ); )
      #else
      stbIF0( stbir__simdf_mult( o0, r0, stbir__if_simdf8_cast_to_simdf4( c0 ) );   stbir__simdf_store( output0, o0 ); )
      stbIF1( stbir__simdf_mult( o0, r0, stbir__if_simdf8_cast_to_simdf4( c1 ) );   stbir__simdf_store( output1, o0 ); )
      stbIF2( stbir__simdf_mult( o0, r0, stbir__if_simdf8_cast_to_simdf4( c2 ) );   stbir__simdf_store( output2, o0 ); )
      stbIF3( stbir__simdf_mult( o0, r0, stbir__if_simdf8_cast_to_simdf4( c3 ) );   stbir__simdf_store( output3, o0 ); )
      stbIF4( stbir__simdf_mult( o0, r0, stbir__if_simdf8_cast_to_simdf4( c4 ) );   stbir__simdf_store( output4, o0 ); )
      stbIF5( stbir__simdf_mult( o0, r0, stbir__if_simdf8_cast_to_simdf4( c5 ) );   stbir__simdf_store( output5, o0 ); )
      stbIF6( stbir__simdf_mult( o0, r0, stbir__if_simdf8_cast_to_simdf4( c6 ) );   stbir__simdf_store( output6, o0 ); )
      stbIF7( stbir__simdf_mult( o0, r0, stbir__if_simdf8_cast_to_simdf4( c7 ) );   stbir__simdf_store( output7, o0 ); )
      #endif

      input += 4;
      stbIF0( output0 += 4; ) stbIF1( output1 += 4; ) stbIF2( output2 += 4; ) stbIF3( output3 += 4; ) stbIF4( output4 += 4; ) stbIF5( output5 += 4; ) stbIF6( output6 += 4; ) stbIF7( output7 += 4; )
    }
  }
  #else
  STBIR_NO_UNROLL_LOOP_START
  while ( ( (char*)input_end - (char*) input ) >= 16 )
  {
    float r0, r1, r2, r3;
    STBIR_NO_UNROLL(input);

    r0 = input[0], r1 = input[1], r2 = input[2], r3 = input[3];

    #ifdef STB_IMAGE_RESIZE_VERTICAL_CONTINUE
    stbIF0( output0[0] += ( r0 * c0s ); output0[1] += ( r1 * c0s ); output0[2] += ( r2 * c0s ); output0[3] += ( r3 * c0s ); )
    stbIF1( output1[0] += ( r0 * c1s ); output1[1] += ( r1 * c1s ); output1[2] += ( r2 * c1s ); output1[3] += ( r3 * c1s ); )
    stbIF2( output2[0] += ( r0 * c2s ); output2[1] += ( r1 * c2s ); output2[2] += ( r2 * c2s ); output2[3] += ( r3 * c2s ); )
    stbIF3( output3[0] += ( r0 * c3s ); output3[1] += ( r1 * c3s ); output3[2] += ( r2 * c3s ); output3[3] += ( r3 * c3s ); )
    stbIF4( output4[0] += ( r0 * c4s ); output4[1] += ( r1 * c4s ); output4[2] += ( r2 * c4s ); output4[3] += ( r3 * c4s ); )
    stbIF5( output5[0] += ( r0 * c5s ); output5[1] += ( r1 * c5s ); output5[2] += ( r2 * c5s ); output5[3] += ( r3 * c5s ); )
    stbIF6( output6[0] += ( r0 * c6s ); output6[1] += ( r1 * c6s ); output6[2] += ( r2 * c6s ); output6[3] += ( r3 * c6s ); )
    stbIF7( output7[0] += ( r0 * c7s ); output7[1] += ( r1 * c7s ); output7[2] += ( r2 * c7s ); output7[3] += ( r3 * c7s ); )
    #else
    stbIF0( output0[0]  = ( r0 * c0s ); output0[1]  = ( r1 * c0s ); output0[2]  = ( r2 * c0s ); output0[3]  = ( r3 * c0s ); )
    stbIF1( output1[0]  = ( r0 * c1s ); output1[1]  = ( r1 * c1s ); output1[2]  = ( r2 * c1s ); output1[3]  = ( r3 * c1s ); )
    stbIF2( output2[0]  = ( r0 * c2s ); output2[1]  = ( r1 * c2s ); output2[2]  = ( r2 * c2s ); output2[3]  = ( r3 * c2s ); )
    stbIF3( output3[0]  = ( r0 * c3s ); output3[1]  = ( r1 * c3s ); output3[2]  = ( r2 * c3s ); output3[3]  = ( r3 * c3s ); )
    stbIF4( output4[0]  = ( r0 * c4s ); output4[1]  = ( r1 * c4s ); output4[2]  = ( r2 * c4s ); output4[3]  = ( r3 * c4s ); )
    stbIF5( output5[0]  = ( r0 * c5s ); output5[1]  = ( r1 * c5s ); output5[2]  = ( r2 * c5s ); output5[3]  = ( r3 * c5s ); )
    stbIF6( output6[0]  = ( r0 * c6s ); output6[1]  = ( r1 * c6s ); output6[2]  = ( r2 * c6s ); output6[3]  = ( r3 * c6s ); )
    stbIF7( output7[0]  = ( r0 * c7s ); output7[1]  = ( r1 * c7s ); output7[2]  = ( r2 * c7s ); output7[3]  = ( r3 * c7s ); )
    #endif

    input += 4;
    stbIF0( output0 += 4; ) stbIF1( output1 += 4; ) stbIF2( output2 += 4; ) stbIF3( output3 += 4; ) stbIF4( output4 += 4; ) stbIF5( output5 += 4; ) stbIF6( output6 += 4; ) stbIF7( output7 += 4; )
  }
  #endif
  STBIR_NO_UNROLL_LOOP_START
  while ( input < input_end )
  {
    float r = input[0];
    STBIR_NO_UNROLL(output0);

    #ifdef STB_IMAGE_RESIZE_VERTICAL_CONTINUE
    stbIF0( output0[0] += ( r * c0s ); )
    stbIF1( output1[0] += ( r * c1s ); )
    stbIF2( output2[0] += ( r * c2s ); )
    stbIF3( output3[0] += ( r * c3s ); )
    stbIF4( output4[0] += ( r * c4s ); )
    stbIF5( output5[0] += ( r * c5s ); )
    stbIF6( output6[0] += ( r * c6s ); )
    stbIF7( output7[0] += ( r * c7s ); )
    #else
    stbIF0( output0[0]  = ( r * c0s ); )
    stbIF1( output1[0]  = ( r * c1s ); )
    stbIF2( output2[0]  = ( r * c2s ); )
    stbIF3( output3[0]  = ( r * c3s ); )
    stbIF4( output4[0]  = ( r * c4s ); )
    stbIF5( output5[0]  = ( r * c5s ); )
    stbIF6( output6[0]  = ( r * c6s ); )
    stbIF7( output7[0]  = ( r * c7s ); )
    #endif

    ++input;
    stbIF0( ++output0; ) stbIF1( ++output1; ) stbIF2( ++output2; ) stbIF3( ++output3; ) stbIF4( ++output4; ) stbIF5( ++output5; ) stbIF6( ++output6; ) stbIF7( ++output7; )
  }
}

static void STBIR_chans( stbir__vertical_gather_with_,_coeffs)( float * outputp, float const * vertical_coefficients, float const ** inputs, float const * input0_end )
{
  float STBIR_SIMD_STREAMOUT_PTR( * ) output = outputp;

  stbIF0( float const * input0 = inputs[0]; float c0s = vertical_coefficients[0]; )
  stbIF1( float const * input1 = inputs[1]; float c1s = vertical_coefficients[1]; )
  stbIF2( float const * input2 = inputs[2]; float c2s = vertical_coefficients[2]; )
  stbIF3( float const * input3 = inputs[3]; float c3s = vertical_coefficients[3]; )
  stbIF4( float const * input4 = inputs[4]; float c4s = vertical_coefficients[4]; )
  stbIF5( float const * input5 = inputs[5]; float c5s = vertical_coefficients[5]; )
  stbIF6( float const * input6 = inputs[6]; float c6s = vertical_coefficients[6]; )
  stbIF7( float const * input7 = inputs[7]; float c7s = vertical_coefficients[7]; )

#if ( STBIR__vertical_channels == 1 ) && !defined(STB_IMAGE_RESIZE_VERTICAL_CONTINUE)
  // check single channel one weight
  if ( ( c0s >= (1.0f-0.000001f) ) && ( c0s <= (1.0f+0.000001f) ) )
  {
    STBIR_MEMCPY( output, input0, (char*)input0_end - (char*)input0 );
    return;
  }
#endif

  #ifdef STBIR_SIMD
  {
    stbIF0(stbir__simdfX c0 = stbir__simdf_frepX( c0s ); )
    stbIF1(stbir__simdfX c1 = stbir__simdf_frepX( c1s ); )
    stbIF2(stbir__simdfX c2 = stbir__simdf_frepX( c2s ); )
    stbIF3(stbir__simdfX c3 = stbir__simdf_frepX( c3s ); )
    stbIF4(stbir__simdfX c4 = stbir__simdf_frepX( c4s ); )
    stbIF5(stbir__simdfX c5 = stbir__simdf_frepX( c5s ); )
    stbIF6(stbir__simdfX c6 = stbir__simdf_frepX( c6s ); )
    stbIF7(stbir__simdfX c7 = stbir__simdf_frepX( c7s ); )

    STBIR_SIMD_NO_UNROLL_LOOP_START
    while ( ( (char*)input0_end - (char*) input0 ) >= (16*stbir__simdfX_float_count) )
    {
      stbir__simdfX o0, o1, o2, o3, r0, r1, r2, r3;
      STBIR_SIMD_NO_UNROLL(output);

      // prefetch four loop iterations ahead (doesn't affect much for small resizes, but helps with big ones)
      stbIF0( stbir__prefetch( input0 + (16*stbir__simdfX_float_count) ); )
      stbIF1( stbir__prefetch( input1 + (16*stbir__simdfX_float_count) ); )
      stbIF2( stbir__prefetch( input2 + (16*stbir__simdfX_float_count) ); )
      stbIF3( stbir__prefetch( input3 + (16*stbir__simdfX_float_count) ); )
      stbIF4( stbir__prefetch( input4 + (16*stbir__simdfX_float_count) ); )
      stbIF5( stbir__prefetch( input5 + (16*stbir__simdfX_float_count) ); )
      stbIF6( stbir__prefetch( input6 + (16*stbir__simdfX_float_count) ); )
      stbIF7( stbir__prefetch( input7 + (16*stbir__simdfX_float_count) ); )

      #ifdef STB_IMAGE_RESIZE_VERTICAL_CONTINUE
      stbIF0( stbir__simdfX_load( o0, output );      stbir__simdfX_load( o1, output+stbir__simdfX_float_count );   stbir__simdfX_load( o2, output+(2*stbir__simdfX_float_count) );   stbir__simdfX_load( o3, output+(3*stbir__simdfX_float_count) );
              stbir__simdfX_load( r0, input0 );      stbir__simdfX_load( r1, input0+stbir__simdfX_float_count );   stbir__simdfX_load( r2, input0+(2*stbir__simdfX_float_count) );   stbir__simdfX_load( r3, input0+(3*stbir__simdfX_float_count) );
              stbir__simdfX_madd( o0, o0, r0, c0 );  stbir__simdfX_madd( o1, o1, r1, c0 );                         stbir__simdfX_madd( o2, o2, r2, c0 );                             stbir__simdfX_madd( o3, o3, r3, c0 ); )
      #else
      stbIF0( stbir__simdfX_load( r0, input0 );      stbir__simdfX_load( r1, input0+stbir__simdfX_float_count );   stbir__simdfX_load( r2, input0+(2*stbir__simdfX_float_count) );   stbir__simdfX_load( r3, input0+(3*stbir__simdfX_float_count) );
              stbir__simdfX_mult( o0, r0, c0 );      stbir__simdfX_mult( o1, r1, c0 );                             stbir__simdfX_mult( o2, r2, c0 );                                 stbir__simdfX_mult( o3, r3, c0 );  )
      #endif

      stbIF1( stbir__simdfX_load( r0, input1 );      stbir__simdfX_load( r1, input1+stbir__simdfX_float_count );   stbir__simdfX_load( r2, input1+(2*stbir__simdfX_float_count) );   stbir__simdfX_load( r3, input1+(3*stbir__simdfX_float_count) );
              stbir__simdfX_madd( o0, o0, r0, c1 );  stbir__simdfX_madd( o1, o1, r1, c1 );                         stbir__simdfX_madd( o2, o2, r2, c1 );                             stbir__simdfX_madd( o3, o3, r3, c1 ); )
      stbIF2( stbir__simdfX_load( r0, input2 );      stbir__simdfX_load( r1, input2+stbir__simdfX_float_count );   stbir__simdfX_load( r2, input2+(2*stbir__simdfX_float_count) );   stbir__simdfX_load( r3, input2+(3*stbir__simdfX_float_count) );
              stbir__simdfX_madd( o0, o0, r0, c2 );  stbir__simdfX_madd( o1, o1, r1, c2 );                         stbir__simdfX_madd( o2, o2, r2, c2 );                             stbir__simdfX_madd( o3, o3, r3, c2 ); )
      stbIF3( stbir__simdfX_load( r0, input3 );      stbir__simdfX_load( r1, input3+stbir__simdfX_float_count );   stbir__simdfX_load( r2, input3+(2*stbir__simdfX_float_count) );   stbir__simdfX_load( r3, input3+(3*stbir__simdfX_float_count) );
              stbir__simdfX_madd( o0, o0, r0, c3 );  stbir__simdfX_madd( o1, o1, r1, c3 );                         stbir__simdfX_madd( o2, o2, r2, c3 );                             stbir__simdfX_madd( o3, o3, r3, c3 ); )
      stbIF4( stbir__simdfX_load( r0, input4 );      stbir__simdfX_load( r1, input4+stbir__simdfX_float_count );   stbir__simdfX_load( r2, input4+(2*stbir__simdfX_float_count) );   stbir__simdfX_load( r3, input4+(3*stbir__simdfX_float_count) );
              stbir__simdfX_madd( o0, o0, r0, c4 );  stbir__simdfX_madd( o1, o1, r1, c4 );                         stbir__simdfX_madd( o2, o2, r2, c4 );                             stbir__simdfX_madd( o3, o3, r3, c4 ); )
      stbIF5( stbir__simdfX_load( r0, input5 );      stbir__simdfX_load( r1, input5+stbir__simdfX_float_count );   stbir__simdfX_load( r2, input5+(2*stbir__simdfX_float_count) );   stbir__simdfX_load( r3, input5+(3*stbir__simdfX_float_count) );
              stbir__simdfX_madd( o0, o0, r0, c5 );  stbir__simdfX_madd( o1, o1, r1, c5 );                         stbir__simdfX_madd( o2, o2, r2, c5 );                             stbir__simdfX_madd( o3, o3, r3, c5 ); )
      stbIF6( stbir__simdfX_load( r0, input6 );      stbir__simdfX_load( r1, input6+stbir__simdfX_float_count );   stbir__simdfX_load( r2, input6+(2*stbir__simdfX_float_count) );   stbir__simdfX_load( r3, input6+(3*stbir__simdfX_float_count) );
              stbir__simdfX_madd( o0, o0, r0, c6 );  stbir__simdfX_madd( o1, o1, r1, c6 );                         stbir__simdfX_madd( o2, o2, r2, c6 );                             stbir__simdfX_madd( o3, o3, r3, c6 ); )
      stbIF7( stbir__simdfX_load( r0, input7 );      stbir__simdfX_load( r1, input7+stbir__simdfX_float_count );   stbir__simdfX_load( r2, input7+(2*stbir__simdfX_float_count) );   stbir__simdfX_load( r3, input7+(3*stbir__simdfX_float_count) );
              stbir__simdfX_madd( o0, o0, r0, c7 );  stbir__simdfX_madd( o1, o1, r1, c7 );                         stbir__simdfX_madd( o2, o2, r2, c7 );                             stbir__simdfX_madd( o3, o3, r3, c7 ); )

      stbir__simdfX_store( output, o0 );             stbir__simdfX_store( output+stbir__simdfX_float_count, o1 );  stbir__simdfX_store( output+(2*stbir__simdfX_float_count), o2 );  stbir__simdfX_store( output+(3*stbir__simdfX_float_count), o3 );
      output += (4*stbir__simdfX_float_count);
      stbIF0( input0 += (4*stbir__simdfX_float_count); ) stbIF1( input1 += (4*stbir__simdfX_float_count); ) stbIF2( input2 += (4*stbir__simdfX_float_count); ) stbIF3( input3 += (4*stbir__simdfX_float_count); ) stbIF4( input4 += (4*stbir__simdfX_float_count); ) stbIF5( input5 += (4*stbir__simdfX_float_count); ) stbIF6( input6 += (4*stbir__simdfX_float_count); ) stbIF7( input7 += (4*stbir__simdfX_float_count); )
    }

    STBIR_SIMD_NO_UNROLL_LOOP_START
    while ( ( (char*)input0_end - (char*) input0 ) >= 16 )
    {
      stbir__simdf o0, r0;
      STBIR_SIMD_NO_UNROLL(output);

      #ifdef STB_IMAGE_RESIZE_VERTICAL_CONTINUE
      stbIF0( stbir__simdf_load( o0, output );   stbir__simdf_load( r0, input0 ); stbir__simdf_madd( o0, o0, r0, stbir__if_simdf8_cast_to_simdf4( c0 ) ); )
      #else
      stbIF0( stbir__simdf_load( r0, input0 );  stbir__simdf_mult( o0, r0, stbir__if_simdf8_cast_to_simdf4( c0 ) ); )
      #endif
      stbIF1( stbir__simdf_load( r0, input1 );  stbir__simdf_madd( o0, o0, r0, stbir__if_simdf8_cast_to_simdf4( c1 ) ); )
      stbIF2( stbir__simdf_load( r0, input2 );  stbir__simdf_madd( o0, o0, r0, stbir__if_simdf8_cast_to_simdf4( c2 ) ); )
      stbIF3( stbir__simdf_load( r0, input3 );  stbir__simdf_madd( o0, o0, r0, stbir__if_simdf8_cast_to_simdf4( c3 ) ); )
      stbIF4( stbir__simdf_load( r0, input4 );  stbir__simdf_madd( o0, o0, r0, stbir__if_simdf8_cast_to_simdf4( c4 ) ); )
      stbIF5( stbir__simdf_load( r0, input5 );  stbir__simdf_madd( o0, o0, r0, stbir__if_simdf8_cast_to_simdf4( c5 ) ); )
      stbIF6( stbir__simdf_load( r0, input6 );  stbir__simdf_madd( o0, o0, r0, stbir__if_simdf8_cast_to_simdf4( c6 ) ); )
      stbIF7( stbir__simdf_load( r0, input7 );  stbir__simdf_madd( o0, o0, r0, stbir__if_simdf8_cast_to_simdf4( c7 ) ); )

      stbir__simdf_store( output, o0 );
      output += 4;
      stbIF0( input0 += 4; ) stbIF1( input1 += 4; ) stbIF2( input2 += 4; ) stbIF3( input3 += 4; ) stbIF4( input4 += 4; ) stbIF5( input5 += 4; ) stbIF6( input6 += 4; ) stbIF7( input7 += 4; )
    }
  }
  #else
  STBIR_NO_UNROLL_LOOP_START
  while ( ( (char*)input0_end - (char*) input0 ) >= 16 )
  {
    float o0, o1, o2, o3;
    STBIR_NO_UNROLL(output);
    #ifdef STB_IMAGE_RESIZE_VERTICAL_CONTINUE
    stbIF0( o0 = output[0] + input0[0] * c0s; o1 = output[1] + input0[1] * c0s; o2 = output[2] + input0[2] * c0s; o3 = output[3] + input0[3] * c0s; )
    #else
    stbIF0( o0  = input0[0] * c0s; o1  = input0[1] * c0s; o2  = input0[2] * c0s; o3  = input0[3] * c0s; )
    #endif
    stbIF1( o0 += input1[0] * c1s; o1 += input1[1] * c1s; o2 += input1[2] * c1s; o3 += input1[3] * c1s; )
    stbIF2( o0 += input2[0] * c2s; o1 += input2[1] * c2s; o2 += input2[2] * c2s; o3 += input2[3] * c2s; )
    stbIF3( o0 += input3[0] * c3s; o1 += input3[1] * c3s; o2 += input3[2] * c3s; o3 += input3[3] * c3s; )
    stbIF4( o0 += input4[0] * c4s; o1 += input4[1] * c4s; o2 += input4[2] * c4s; o3 += input4[3] * c4s; )
    stbIF5( o0 += input5[0] * c5s; o1 += input5[1] * c5s; o2 += input5[2] * c5s; o3 += input5[3] * c5s; )
    stbIF6( o0 += input6[0] * c6s; o1 += input6[1] * c6s; o2 += input6[2] * c6s; o3 += input6[3] * c6s; )
    stbIF7( o0 += input7[0] * c7s; o1 += input7[1] * c7s; o2 += input7[2] * c7s; o3 += input7[3] * c7s; )
    output[0] = o0; output[1] = o1; output[2] = o2; output[3] = o3;
    output += 4;
    stbIF0( input0 += 4; ) stbIF1( input1 += 4; ) stbIF2( input2 += 4; ) stbIF3( input3 += 4; ) stbIF4( input4 += 4; ) stbIF5( input5 += 4; ) stbIF6( input6 += 4; ) stbIF7( input7 += 4; )
  }
  #endif
  STBIR_NO_UNROLL_LOOP_START
  while ( input0 < input0_end )
  {
    float o0;
    STBIR_NO_UNROLL(output);
    #ifdef STB_IMAGE_RESIZE_VERTICAL_CONTINUE
    stbIF0( o0 = output[0] + input0[0] * c0s; )
    #else
    stbIF0( o0  = input0[0] * c0s; )
    #endif
    stbIF1( o0 += input1[0] * c1s; )
    stbIF2( o0 += input2[0] * c2s; )
    stbIF3( o0 += input3[0] * c3s; )
    stbIF4( o0 += input4[0] * c4s; )
    stbIF5( o0 += input5[0] * c5s; )
    stbIF6( o0 += input6[0] * c6s; )
    stbIF7( o0 += input7[0] * c7s; )
    output[0] = o0;
    ++output;
    stbIF0( ++input0; ) stbIF1( ++input1; ) stbIF2( ++input2; ) stbIF3( ++input3; ) stbIF4( ++input4; ) stbIF5( ++input5; ) stbIF6( ++input6; ) stbIF7( ++input7; )
  }
}

#undef stbIF0
#undef stbIF1
#undef stbIF2
#undef stbIF3
#undef stbIF4
#undef stbIF5
#undef stbIF6
#undef stbIF7
#undef STB_IMAGE_RESIZE_DO_VERTICALS
#undef STBIR__vertical_channels
#undef STB_IMAGE_RESIZE_DO_HORIZONTALS
#undef STBIR_strs_join24
#undef STBIR_strs_join14
#undef STBIR_chans
#ifdef STB_IMAGE_RESIZE_VERTICAL_CONTINUE
#undef STB_IMAGE_RESIZE_VERTICAL_CONTINUE
#endif

#else // !STB_IMAGE_RESIZE_DO_VERTICALS

#define STBIR_chans( start, end ) STBIR_strs_join1(start,STBIR__horizontal_channels,end)

#ifndef stbir__2_coeff_only
#define stbir__2_coeff_only()             \
    stbir__1_coeff_only();                \
    stbir__1_coeff_remnant(1);
#endif

#ifndef stbir__2_coeff_remnant
#define stbir__2_coeff_remnant( ofs )     \
    stbir__1_coeff_remnant(ofs);          \
    stbir__1_coeff_remnant((ofs)+1);
#endif

#ifndef stbir__3_coeff_only
#define stbir__3_coeff_only()             \
    stbir__2_coeff_only();                \
    stbir__1_coeff_remnant(2);
#endif

#ifndef stbir__3_coeff_remnant
#define stbir__3_coeff_remnant( ofs )     \
    stbir__2_coeff_remnant(ofs);          \
    stbir__1_coeff_remnant((ofs)+2);
#endif

#ifndef stbir__3_coeff_setup
#define stbir__3_coeff_setup()
#endif

#ifndef stbir__4_coeff_start
#define stbir__4_coeff_start()            \
    stbir__2_coeff_only();                \
    stbir__2_coeff_remnant(2);
#endif

#ifndef stbir__4_coeff_continue_from_4
#define stbir__4_coeff_continue_from_4( ofs )     \
    stbir__2_coeff_remnant(ofs);                  \
    stbir__2_coeff_remnant((ofs)+2);
#endif

#ifndef stbir__store_output_tiny
#define stbir__store_output_tiny stbir__store_output
#endif

static void STBIR_chans( stbir__horizontal_gather_,_channels_with_1_coeff)( float * output_buffer, unsigned int output_sub_size, float const * decode_buffer, stbir__contributors const * horizontal_contributors, float const * horizontal_coefficients, int coefficient_width )
{
  float const * output_end = output_buffer + output_sub_size * STBIR__horizontal_channels;
  float STBIR_SIMD_STREAMOUT_PTR( * ) output = output_buffer;
  STBIR_SIMD_NO_UNROLL_LOOP_START
  do {
    float const * decode = decode_buffer + horizontal_contributors->n0 * STBIR__horizontal_channels;
    float const * hc = horizontal_coefficients;
    stbir__1_coeff_only();
    stbir__store_output_tiny();
  } while ( output < output_end );
}

static void STBIR_chans( stbir__horizontal_gather_,_channels_with_2_coeffs)( float * output_buffer, unsigned int output_sub_size, float const * decode_buffer, stbir__contributors const * horizontal_contributors, float const * horizontal_coefficients, int coefficient_width )
{
  float const * output_end = output_buffer + output_sub_size * STBIR__horizontal_channels;
  float STBIR_SIMD_STREAMOUT_PTR( * ) output = output_buffer;
  STBIR_SIMD_NO_UNROLL_LOOP_START
  do {
    float const * decode = decode_buffer + horizontal_contributors->n0 * STBIR__horizontal_channels;
    float const * hc = horizontal_coefficients;
    stbir__2_coeff_only();
    stbir__store_output_tiny();
  } while ( output < output_end );
}

static void STBIR_chans( stbir__horizontal_gather_,_channels_with_3_coeffs)( float * output_buffer, unsigned int output_sub_size, float const * decode_buffer, stbir__contributors const * horizontal_contributors, float const * horizontal_coefficients, int coefficient_width )
{
  float const * output_end = output_buffer + output_sub_size * STBIR__horizontal_channels;
  float STBIR_SIMD_STREAMOUT_PTR( * ) output = output_buffer;
  STBIR_SIMD_NO_UNROLL_LOOP_START
  do {
    float const * decode = decode_buffer + horizontal_contributors->n0 * STBIR__horizontal_channels;
    float const * hc = horizontal_coefficients;
    stbir__3_coeff_only();
    stbir__store_output_tiny();
  } while ( output < output_end );
}

static void STBIR_chans( stbir__horizontal_gather_,_channels_with_4_coeffs)( float * output_buffer, unsigned int output_sub_size, float const * decode_buffer, stbir__contributors const * horizontal_contributors, float const * horizontal_coefficients, int coefficient_width )
{
  float const * output_end = output_buffer + output_sub_size * STBIR__horizontal_channels;
  float STBIR_SIMD_STREAMOUT_PTR( * ) output = output_buffer;
  STBIR_SIMD_NO_UNROLL_LOOP_START
  do {
    float const * decode = decode_buffer + horizontal_contributors->n0 * STBIR__horizontal_channels;
    float const * hc = horizontal_coefficients;
    stbir__4_coeff_start();
    stbir__store_output();
  } while ( output < output_end );
}

static void STBIR_chans( stbir__horizontal_gather_,_channels_with_5_coeffs)( float * output_buffer, unsigned int output_sub_size, float const * decode_buffer, stbir__contributors const * horizontal_contributors, float const * horizontal_coefficients, int coefficient_width )
{
  float const * output_end = output_buffer + output_sub_size * STBIR__horizontal_channels;
  float STBIR_SIMD_STREAMOUT_PTR( * ) output = output_buffer;
  STBIR_SIMD_NO_UNROLL_LOOP_START
  do {
    float const * decode = decode_buffer + horizontal_contributors->n0 * STBIR__horizontal_channels;
    float const * hc = horizontal_coefficients;
    stbir__4_coeff_start();
    stbir__1_coeff_remnant(4);
    stbir__store_output();
  } while ( output < output_end );
}

static void STBIR_chans( stbir__horizontal_gather_,_channels_with_6_coeffs)( float * output_buffer, unsigned int output_sub_size, float const * decode_buffer, stbir__contributors const * horizontal_contributors, float const * horizontal_coefficients, int coefficient_width )
{
  float const * output_end = output_buffer + output_sub_size * STBIR__horizontal_channels;
  float STBIR_SIMD_STREAMOUT_PTR( * ) output = output_buffer;
  STBIR_SIMD_NO_UNROLL_LOOP_START
  do {
    float const * decode = decode_buffer + horizontal_contributors->n0 * STBIR__horizontal_channels;
    float const * hc = horizontal_coefficients;
    stbir__4_coeff_start();
    stbir__2_coeff_remnant(4);
    stbir__store_output();
  } while ( output < output_end );
}

static void STBIR_chans( stbir__horizontal_gather_,_channels_with_7_coeffs)( float * output_buffer, unsigned int output_sub_size, float const * decode_buffer, stbir__contributors const * horizontal_contributors, float const * horizontal_coefficients, int coefficient_width )
{
  float const * output_end = output_buffer + output_sub_size * STBIR__horizontal_channels;
  float STBIR_SIMD_STREAMOUT_PTR( * ) output = output_buffer;
  stbir__3_coeff_setup();
  STBIR_SIMD_NO_UNROLL_LOOP_START
  do {
    float const * decode = decode_buffer + horizontal_contributors->n0 * STBIR__horizontal_channels;
    float const * hc = horizontal_coefficients;

    stbir__4_coeff_start();
    stbir__3_coeff_remnant(4);
    stbir__store_output();
  } while ( output < output_end );
}

static void STBIR_chans( stbir__horizontal_gather_,_channels_with_8_coeffs)( float * output_buffer, unsigned int output_sub_size, float const * decode_buffer, stbir__contributors const * horizontal_contributors, float const * horizontal_coefficients, int coefficient_width )
{
  float const * output_end = output_buffer + output_sub_size * STBIR__horizontal_channels;
  float STBIR_SIMD_STREAMOUT_PTR( * ) output = output_buffer;
  STBIR_SIMD_NO_UNROLL_LOOP_START
  do {
    float const * decode = decode_buffer + horizontal_contributors->n0 * STBIR__horizontal_channels;
    float const * hc = horizontal_coefficients;
    stbir__4_coeff_start();
    stbir__4_coeff_continue_from_4(4);
    stbir__store_output();
  } while ( output < output_end );
}

static void STBIR_chans( stbir__horizontal_gather_,_channels_with_9_coeffs)( float * output_buffer, unsigned int output_sub_size, float const * decode_buffer, stbir__contributors const * horizontal_contributors, float const * horizontal_coefficients, int coefficient_width )
{
  float const * output_end = output_buffer + output_sub_size * STBIR__horizontal_channels;
  float STBIR_SIMD_STREAMOUT_PTR( * ) output = output_buffer;
  STBIR_SIMD_NO_UNROLL_LOOP_START
  do {
    float const * decode = decode_buffer + horizontal_contributors->n0 * STBIR__horizontal_channels;
    float const * hc = horizontal_coefficients;
    stbir__4_coeff_start();
    stbir__4_coeff_continue_from_4(4);
    stbir__1_coeff_remnant(8);
    stbir__store_output();
  } while ( output < output_end );
}

static void STBIR_chans( stbir__horizontal_gather_,_channels_with_10_coeffs)( float * output_buffer, unsigned int output_sub_size, float const * decode_buffer, stbir__contributors const * horizontal_contributors, float const * horizontal_coefficients, int coefficient_width )
{
  float const * output_end = output_buffer + output_sub_size * STBIR__horizontal_channels;
  float STBIR_SIMD_STREAMOUT_PTR( * ) output = output_buffer;
  STBIR_SIMD_NO_UNROLL_LOOP_START
  do {
    float const * decode = decode_buffer + horizontal_contributors->n0 * STBIR__horizontal_channels;
    float const * hc = horizontal_coefficients;
    stbir__4_coeff_start();
    stbir__4_coeff_continue_from_4(4);
    stbir__2_coeff_remnant(8);
    stbir__store_output();
  } while ( output < output_end );
}

static void STBIR_chans( stbir__horizontal_gather_,_channels_with_11_coeffs)( float * output_buffer, unsigned int output_sub_size, float const * decode_buffer, stbir__contributors const * horizontal_contributors, float const * horizontal_coefficients, int coefficient_width )
{
  float const * output_end = output_buffer + output_sub_size * STBIR__horizontal_channels;
  float STBIR_SIMD_STREAMOUT_PTR( * ) output = output_buffer;
  stbir__3_coeff_setup();
  STBIR_SIMD_NO_UNROLL_LOOP_START
  do {
    float const * decode = decode_buffer + horizontal_contributors->n0 * STBIR__horizontal_channels;
    float const * hc = horizontal_coefficients;
    stbir__4_coeff_start();
    stbir__4_coeff_continue_from_4(4);
    stbir__3_coeff_remnant(8);
    stbir__store_output();
  } while ( output < output_end );
}

static void STBIR_chans( stbir__horizontal_gather_,_channels_with_12_coeffs)( float * output_buffer, unsigned int output_sub_size, float const * decode_buffer, stbir__contributors const * horizontal_contributors, float const * horizontal_coefficients, int coefficient_width )
{
  float const * output_end = output_buffer + output_sub_size * STBIR__horizontal_channels;
  float STBIR_SIMD_STREAMOUT_PTR( * ) output = output_buffer;
  STBIR_SIMD_NO_UNROLL_LOOP_START
  do {
    float const * decode = decode_buffer + horizontal_contributors->n0 * STBIR__horizontal_channels;
    float const * hc = horizontal_coefficients;
    stbir__4_coeff_start();
    stbir__4_coeff_continue_from_4(4);
    stbir__4_coeff_continue_from_4(8);
    stbir__store_output();
  } while ( output < output_end );
}

static void STBIR_chans( stbir__horizontal_gather_,_channels_with_n_coeffs_mod0 )( float * output_buffer, unsigned int output_sub_size, float const * decode_buffer, stbir__contributors const * horizontal_contributors, float const * horizontal_coefficients, int coefficient_width )
{
  float const * output_end = output_buffer + output_sub_size * STBIR__horizontal_channels;
  float STBIR_SIMD_STREAMOUT_PTR( * ) output = output_buffer;
  STBIR_SIMD_NO_UNROLL_LOOP_START
  do {
    float const * decode = decode_buffer + horizontal_contributors->n0 * STBIR__horizontal_channels;
    int n = ( ( horizontal_contributors->n1 - horizontal_contributors->n0 + 1 ) - 4 + 3 ) >> 2;
    float const * hc = horizontal_coefficients;

    stbir__4_coeff_start();
    STBIR_SIMD_NO_UNROLL_LOOP_START
    do {
      hc += 4;
      decode += STBIR__horizontal_channels * 4;
      stbir__4_coeff_continue_from_4( 0 );
      --n;
    } while ( n > 0 );
    stbir__store_output();
  } while ( output < output_end );
}

static void STBIR_chans( stbir__horizontal_gather_,_channels_with_n_coeffs_mod1 )( float * output_buffer, unsigned int output_sub_size, float const * decode_buffer, stbir__contributors const * horizontal_contributors, float const * horizontal_coefficients, int coefficient_width )
{
  float const * output_end = output_buffer + output_sub_size * STBIR__horizontal_channels;
  float STBIR_SIMD_STREAMOUT_PTR( * ) output = output_buffer;
  STBIR_SIMD_NO_UNROLL_LOOP_START
  do {
    float const * decode = decode_buffer + horizontal_contributors->n0 * STBIR__horizontal_channels;
    int n = ( ( horizontal_contributors->n1 - horizontal_contributors->n0 + 1 ) - 5 + 3 ) >> 2;
    float const * hc = horizontal_coefficients;

    stbir__4_coeff_start();
    STBIR_SIMD_NO_UNROLL_LOOP_START
    do {
      hc += 4;
      decode += STBIR__horizontal_channels * 4;
      stbir__4_coeff_continue_from_4( 0 );
      --n;
    } while ( n > 0 );
    stbir__1_coeff_remnant( 4 );
    stbir__store_output();
  } while ( output < output_end );
}

static void STBIR_chans( stbir__horizontal_gather_,_channels_with_n_coeffs_mod2 )( float * output_buffer, unsigned int output_sub_size, float const * decode_buffer, stbir__contributors const * horizontal_contributors, float const * horizontal_coefficients, int coefficient_width )
{
  float const * output_end = output_buffer + output_sub_size * STBIR__horizontal_channels;
  float STBIR_SIMD_STREAMOUT_PTR( * ) output = output_buffer;
  STBIR_SIMD_NO_UNROLL_LOOP_START
  do {
    float const * decode = decode_buffer + horizontal_contributors->n0 * STBIR__horizontal_channels;
    int n = ( ( horizontal_contributors->n1 - horizontal_contributors->n0 + 1 ) - 6 + 3 ) >> 2;
    float const * hc = horizontal_coefficients;

    stbir__4_coeff_start();
    STBIR_SIMD_NO_UNROLL_LOOP_START
    do {
      hc += 4;
      decode += STBIR__horizontal_channels * 4;
      stbir__4_coeff_continue_from_4( 0 );
      --n;
    } while ( n > 0 );
    stbir__2_coeff_remnant( 4 );

    stbir__store_output();
  } while ( output < output_end );
}

static void STBIR_chans( stbir__horizontal_gather_,_channels_with_n_coeffs_mod3 )( float * output_buffer, unsigned int output_sub_size, float const * decode_buffer, stbir__contributors const * horizontal_contributors, float const * horizontal_coefficients, int coefficient_width )
{
  float const * output_end = output_buffer + output_sub_size * STBIR__horizontal_channels;
  float STBIR_SIMD_STREAMOUT_PTR( * ) output = output_buffer;
  stbir__3_coeff_setup();
  STBIR_SIMD_NO_UNROLL_LOOP_START
  do {
    float const * decode = decode_buffer + horizontal_contributors->n0 * STBIR__horizontal_channels;
    int n = ( ( horizontal_contributors->n1 - horizontal_contributors->n0 + 1 ) - 7 + 3 ) >> 2;
    float const * hc = horizontal_coefficients;

    stbir__4_coeff_start();
    STBIR_SIMD_NO_UNROLL_LOOP_START
    do {
      hc += 4;
      decode += STBIR__horizontal_channels * 4;
      stbir__4_coeff_continue_from_4( 0 );
      --n;
    } while ( n > 0 );
    stbir__3_coeff_remnant( 4 );

    stbir__store_output();
  } while ( output < output_end );
}

static stbir__horizontal_gather_channels_func * STBIR_chans(stbir__horizontal_gather_,_channels_with_n_coeffs_funcs)[4]=
{
  STBIR_chans(stbir__horizontal_gather_,_channels_with_n_coeffs_mod0),
  STBIR_chans(stbir__horizontal_gather_,_channels_with_n_coeffs_mod1),
  STBIR_chans(stbir__horizontal_gather_,_channels_with_n_coeffs_mod2),
  STBIR_chans(stbir__horizontal_gather_,_channels_with_n_coeffs_mod3),
};

static stbir__horizontal_gather_channels_func * STBIR_chans(stbir__horizontal_gather_,_channels_funcs)[12]=
{
  STBIR_chans(stbir__horizontal_gather_,_channels_with_1_coeff),
  STBIR_chans(stbir__horizontal_gather_,_channels_with_2_coeffs),
  STBIR_chans(stbir__horizontal_gather_,_channels_with_3_coeffs),
  STBIR_chans(stbir__horizontal_gather_,_channels_with_4_coeffs),
  STBIR_chans(stbir__horizontal_gather_,_channels_with_5_coeffs),
  STBIR_chans(stbir__horizontal_gather_,_channels_with_6_coeffs),
  STBIR_chans(stbir__horizontal_gather_,_channels_with_7_coeffs),
  STBIR_chans(stbir__horizontal_gather_,_channels_with_8_coeffs),
  STBIR_chans(stbir__horizontal_gather_,_channels_with_9_coeffs),
  STBIR_chans(stbir__horizontal_gather_,_channels_with_10_coeffs),
  STBIR_chans(stbir__horizontal_gather_,_channels_with_11_coeffs),
  STBIR_chans(stbir__horizontal_gather_,_channels_with_12_coeffs),
};

#undef STBIR__horizontal_channels
#undef STB_IMAGE_RESIZE_DO_HORIZONTALS
#undef stbir__1_coeff_only
#undef stbir__1_coeff_remnant
#undef stbir__2_coeff_only
#undef stbir__2_coeff_remnant
#undef stbir__3_coeff_only
#undef stbir__3_coeff_remnant
#undef stbir__3_coeff_setup
#undef stbir__4_coeff_start
#undef stbir__4_coeff_continue_from_4
#undef stbir__store_output
#undef stbir__store_output_tiny
#undef STBIR_chans

#endif  // HORIZONALS

#undef STBIR_strs_join2
#undef STBIR_strs_join1
