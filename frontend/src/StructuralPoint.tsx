import React, { useState } from "react";
import { Button, Col, Form, Row, Table } from "react-bootstrap";
import { RouteComponentProps, useHistory } from "react-router-dom";
import axios from 'axios';
import { DbStructuralPoint } from "./DatabaseInterface";

export type StructuralPointProps = {
    projectId: string;
}

export function StructuralPoint(props: StructuralPointProps) {
    const history = useHistory();

    const onBack = () => { history.push("/structure/" + props.projectId); }
    // const onSave = () => { history.push("/new-material/" + props.projectId); }

    const [data, setData] = useState<DbStructuralPoint>({
        id: 0,
        uuid: "",
        name: "",
        coordinate_x: 0,
        coordinate_y: 0,
        coordinate_z: 0,
    })

    const [validated, setValidated] = useState(false);
    const [status, setStatus] = useState('');

    const handleSubmit = (event: { currentTarget: any; preventDefault: () => void; stopPropagation: () => void; }) => {
        const form = event.currentTarget;
        if (form.checkValidity() === false) {
            event.preventDefault();
            event.stopPropagation();
        }

        event.preventDefault();
        console.log(data);     

        axios.post(process.env.BACKEND + '/structuralpoints/' + props.projectId, data)
            .then(() => { onBack() });

        console.log("status: ", status);        
        // onBack()    
    };

    const handleInputChange = (e: { target: { name: any; value: any; }; }) => {
        const { name, value } = e.target;
        setData({ ...data, [name]: value, });
    };

    return (
        <Form noValidate validated={validated} onSubmit={handleSubmit}>
            <Form.Row>
                <Form.Group as={Col} md="4" controlId="validationCustom01">
                    <Form.Label>Name</Form.Label>
                    <Form.Control
                        required
                        type="text"
                        name="name"
                        placeholder="Name"
                        value={data.name}
                        onChange={handleInputChange}
                    />
                    <Form.Control.Feedback>Please provide a valid name.</Form.Control.Feedback>
                </Form.Group>
                <Form.Group as={Col} md="4" controlId="validationCustom02">
                    <Form.Label>X</Form.Label>
                    <Form.Control
                        required
                        type="text"
                        name="coordinate_x"
                        placeholder="X coordinate"
                        value={data.coordinate_x}
                        onChange={handleInputChange}
                    />
                    <Form.Control.Feedback>Please provide a valid value.</Form.Control.Feedback>
                </Form.Group>
                <Form.Group as={Col} md="4" controlId="validationCustom03">
                    <Form.Label>Y</Form.Label>
                    <Form.Control
                        required
                        type="text"
                        name="coordinate_y"
                        placeholder="Y coordinate in UCS"
                        value={data.coordinate_y}
                        onChange={handleInputChange}
                    />
                    <Form.Control.Feedback>Please provide a valid value.</Form.Control.Feedback>
                </Form.Group>
                <Form.Group as={Col} md="4" controlId="validationCustom04">
                    <Form.Label>Z</Form.Label>
                    <Form.Control
                        required
                        type="text"
                        name="coordinate_z"
                        placeholder="Z coordinate in UCS"
                        value={data.coordinate_z}
                        onChange={handleInputChange}
                    />
                    <Form.Control.Feedback>Please provide a valid value.</Form.Control.Feedback>
                </Form.Group>
            </Form.Row>
            <Button type="submit">Submit form</Button>
        </Form>        
    )
}

export interface RouterComponentStructuralPointProps extends RouteComponentProps<StructuralPointProps> { }
