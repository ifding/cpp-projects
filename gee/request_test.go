package gee

import (
	"errors"
	"reflect"
	"testing"
)

func TestQueryEncoder(t *testing.T) {
	want := "Test=http%3A%2F%2Fgithub.com"
	got := QueryEncoder(map[string]string{
		"Test": "http://github.com",
	})
	if !reflect.DeepEqual(want, got) {
		t.Fatalf("want %v, got: %v", want, got)
	}
}

func TestHTTPRequest(t *testing.T) {
	got := HTTPRequest("https://google.com/", "GET", []byte{}, &RequestParams{
		Timeout: -100,
		AuthUser: "test",
		AuthPass: "test",
	}, &struct{}{})
	if errors.Is(nil, got) {
		t.Fatalf("want error, got: %v", got)
	}
}