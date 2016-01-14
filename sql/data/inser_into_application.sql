DO
$do$
BEGIN 
FOR i IN 1..1000000 LOOP


INSERT INTO application (
	bdoc,
	data,
	date_created,
	uuid
) VALUES (
'base64String',
'{
	"object_type_uuid":"a3a817c5-e19a-4079-905d-65cb1f608455",
	"identity_uuid":"be7e1952-6dad-11e5-9d70-feff819cdc9f",
	"date":"2015-10-29 10:02:20.100+02",
	"application_nr":"1000000076/MID",
	"identity":{			
		"firstname":"Peeter",
		"lastname":"Moor",	
		"personcode":"38111035888"	
	},
	"sim":{
			"sim_nr":"89372152061370399125",
			"mobile_nr":"37299629449",
			"operator":"EMT",
			"valid_to":"2015-10-29 23:59:59.999+02"
		},
	"request_uusis_date":"2015-10-29 00:00:00.000+02",
	"request_sk_date":"2015-10-29 00:00:00.000+02",
	"classifier_application_source": {
		"uuid":"a3a337c5-e19a-3079-905d-65cb1f608455",
		"code":"KANAL_T",
		"label":"Taotluskeskkond"
	}
}',
current_timestamp,
uuid_in(md5(random()::text || now()::text)::cstring)

);

END LOOP;
END
$do$;